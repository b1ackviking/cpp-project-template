#pragma once
#include "coro/awaitable_traits.hpp"

#include <atomic>
#include <coroutine>
#include <cstddef>
#include <exception>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace coro::detail {
class when_all_counter {
public:
  explicit when_all_counter(std::size_t count)
    : count_{count}, continuation_{} {}

  [[nodiscard]] bool is_ready() const noexcept {
    return static_cast<bool>(continuation_);
  }

  [[nodiscard]] bool try_await(std::coroutine_handle<> h) noexcept {
    continuation_ = h;
    return count_.fetch_sub(1) > 0;
  }

  std::coroutine_handle<> notify_completed() noexcept {
    if (count_.fetch_sub(1) == 0) {
      return continuation_;
    }
    return std::noop_coroutine();
  }

private:
  std::atomic_size_t count_;
  std::coroutine_handle<> continuation_{std::noop_coroutine()};
};

template<typename T>
class when_all_ready_awaitable;

template<>
class when_all_ready_awaitable<std::tuple<>> {
public:
  constexpr explicit when_all_ready_awaitable() = default;
  constexpr explicit when_all_ready_awaitable(std::tuple<>) {}
  constexpr bool await_ready() const noexcept { return true; }
  constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
  constexpr std::tuple<> await_resume() const noexcept { return {}; }
};

template<typename... Tasks>
class when_all_ready_awaitable<std::tuple<Tasks...>> {
public:
  explicit when_all_ready_awaitable(Tasks&&... tasks) noexcept(
    std::conjunction_v<std::is_nothrow_move_constructible<Tasks>...>)
    : counter_{sizeof...(Tasks)}, tasks_{std::move(tasks)...} {}

  explicit when_all_ready_awaitable(std::tuple<Tasks...>&& tasks) noexcept(
    std::is_nothrow_move_constructible_v<std::tuple<Tasks...>>)
    : counter_{sizeof...(Tasks)}, tasks_{std::move(tasks)} {}

  when_all_ready_awaitable(const when_all_ready_awaitable&) = delete;
  when_all_ready_awaitable& operator=(const when_all_ready_awaitable&) = delete;
  when_all_ready_awaitable(when_all_ready_awaitable&& other) noexcept(
    std::is_nothrow_move_constructible_v<std::tuple<Tasks...>>)
    : counter_{sizeof...(Tasks)}, tasks_{std::move(other.tasks_)} {}
  when_all_ready_awaitable& operator=(when_all_ready_awaitable&&) = delete;
  ~when_all_ready_awaitable() = default;

  auto operator co_await() & noexcept {
    struct awaiter {
      bool await_ready() const noexcept { return awaitable.is_ready(); }
      bool await_suspend(std::coroutine_handle<> h) noexcept {
        return awaitable.try_await(h);
      }
      std::tuple<Tasks...>& await_resume() noexcept { return awaitable.tasks_; }
      when_all_ready_awaitable& awaitable;
    };
    return awaiter{*this};
  }

  auto operator co_await() && noexcept {
    struct awaiter {
      bool await_ready() const noexcept { return awaitable.is_ready(); }
      bool await_suspend(std::coroutine_handle<> h) noexcept {
        return awaitable.try_await(h);
      }
      std::tuple<Tasks...>&& await_resume() noexcept {
        return std::move(awaitable.tasks_);
      }
      when_all_ready_awaitable& awaitable;
    };
    return awaiter{*this};
  }

private:
  when_all_counter counter_;
  std::tuple<Tasks...> tasks_;

  bool is_ready() const noexcept { return counter_.is_ready(); }

  bool try_await(std::coroutine_handle<> h) noexcept {
    [this]<std::size_t... is>(std::index_sequence<is...>) {
      (..., std::get<is>(tasks_).start(counter_));
    }
    (std::make_index_sequence<sizeof...(Tasks)>{});
    return counter_.try_await(h);
  }
};

template<typename T>
class when_all_task;

template<typename T>
class when_all_promise final {
  using handle_t = std::coroutine_handle<when_all_promise<T>>;

public:
  auto get_return_object() noexcept { return handle_t::from_promise(*this); };

  std::suspend_always initial_suspend() const noexcept { return {}; }
  auto final_suspend() noexcept {
    struct completion_notifier {
      bool await_ready() const noexcept { return false; }
      std::coroutine_handle<> await_suspend(handle_t h) noexcept {
        return h.promise().counter_->notify_completed();
      }
      void await_resume() const noexcept {}
    };
    return completion_notifier{};
  }
  void unhandled_exception() { value = std::current_exception(); }
  void return_void() { std::unreachable(); }
  template<std::convertible_to<T> V>
  auto yield_value(V&& v) noexcept(std::is_nothrow_convertible_v<V, T>) {
    value = std::forward<V>(v);
    return final_suspend();
  }
  void start(when_all_counter& counter) {
    counter_ = &counter;
    handle_t::from_promise(*this).resume();
  }

  [[nodiscard]] T& result() & {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return std::get<std::remove_reference_t<T>>(value);
  }

  [[nodiscard]] T&& result() && {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(std::move(value)));
    }
    return std::get<std::remove_reference_t<T>>(std::move(value));
  }

private:
  when_all_counter* counter_;
  std::variant<std::monostate, std::remove_reference_t<T>, std::exception_ptr>
    value;
};

template<>
class when_all_promise<void> final {
  using handle_t = std::coroutine_handle<when_all_promise<void>>;

public:
  auto get_return_object() noexcept { return handle_t::from_promise(*this); };

  std::suspend_always initial_suspend() const noexcept { return {}; }
  auto final_suspend() noexcept {
    struct completion_notifier {
      bool await_ready() const noexcept { return false; }
      std::coroutine_handle<> await_suspend(handle_t h) noexcept {
        return h.promise().counter_->notify_completed();
      }
      void await_resume() const noexcept {}
    };
    return completion_notifier{};
  }
  void unhandled_exception() { exception_ = std::current_exception(); }
  void return_void() {}
  void start(when_all_counter& counter) {
    counter_ = &counter;
    handle_t::from_promise(*this).resume();
  }

  void result() {
    if (exception_) {
      std::rethrow_exception(exception_);
    }
  }

private:
  when_all_counter* counter_;
  std::exception_ptr exception_;
};

template<typename T>
class when_all_task {
  template<typename TasksContainer>
  friend class when_all_ready_awaitable;

public:
  using promise_type = when_all_promise<T>;

  when_all_task(std::coroutine_handle<promise_type> h) : ch_{h} {}
  when_all_task(const when_all_task&) = delete;
  when_all_task& operator=(const when_all_task&) = delete;
  when_all_task(when_all_task&& other) noexcept
    : ch_{std::exchange(other.ch_, nullptr)} {}
  when_all_task& operator=(when_all_task&& other) noexcept {
    if (std::addressof(other) != this) {
      ch_ = std::exchange(other.ch_, nullptr);
    }
    return *this;
  }
  ~when_all_task() noexcept {
    if (ch_) {
      ch_.destroy();
    }
  }

  decltype(auto) result() & { return ch_.promise().result(); }
  decltype(auto) result() && { return std::move(ch_.promise()).result(); }

private:
  std::coroutine_handle<promise_type> ch_;

  void start(when_all_counter& counter) noexcept {
    ch_.promise().start(counter);
  }
};

template<awaitable T,
         typename Result = typename awaitable_traits<T&&>::await_result_t>
when_all_task<Result> make_when_all_task(T awaitable) {
  co_yield co_await std::move(awaitable);
}

template<awaitable T, std::same_as<void> Result =
                        typename awaitable_traits<T&&>::await_result_t>
when_all_task<void> make_when_all_task(T awaitable) {
  co_await std::move(awaitable);
}
} // namespace coro::detail

namespace coro {
template<awaitable... Ts>
auto when_all_ready(Ts&&... ts) {
  return detail::when_all_ready_awaitable<
    std::tuple<detail::when_all_task<typename awaitable_traits<
      std::remove_reference_t<Ts>>::await_result_t>...>>{
    std::forward_as_tuple(detail::make_when_all_task(std::forward<Ts>(ts))...)};
}
} // namespace coro
