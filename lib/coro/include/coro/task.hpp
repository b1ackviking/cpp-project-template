#pragma once
#include <concepts>
#include <coroutine>
#include <exception>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

namespace coro {
class broken_promise : public std::logic_error {
public:
  broken_promise() : std::logic_error{"broken promise"} {}
};

template<typename T>
class task;

namespace detail {
struct promise_base {
  std::coroutine_handle<> continuation{std::noop_coroutine()};

  struct final_awaitable {
    bool await_ready() const noexcept { return false; }
    template<typename T>
    std::coroutine_handle<> await_suspend(std::coroutine_handle<T> h) noexcept {
      return h.promise().continuation;
    }
    void await_resume() const noexcept {}
  };

  std::suspend_always initial_suspend() const noexcept { return {}; }
  final_awaitable final_suspend() const noexcept { return {}; }
};

template<typename T>
struct promise : promise_base {
  std::variant<std::monostate, std::exception_ptr, T> value;

  task<T> get_return_object() noexcept;
  void unhandled_exception() { value = std::current_exception(); }

  template<std::convertible_to<T> V>
  void return_value(V&& v) noexcept(std::is_nothrow_convertible_v<V, T>) {
    value = std::forward<V>(v);
  }

  [[nodiscard]] T& result() & {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return std::get<T>(value);
  }

  [[nodiscard]] T&& result() && {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(std::move(value)));
    }
    return std::get<T>(std::move(value));
  }
};

template<typename T>
struct promise<T&> : promise_base {
  std::variant<std::monostate, std::exception_ptr, T*> value;

  task<T&> get_return_object() noexcept;
  void unhandled_exception() { value = std::current_exception(); }
  void return_value(T& v) noexcept { value = std::addressof(v); }
  T& result() {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return *std::get<T*>(value);
  }
};

template<>
struct promise<void> : promise_base {
  std::exception_ptr exception;

  task<void> get_return_object() noexcept;
  void unhandled_exception() { exception = std::current_exception(); }
  void return_void() const noexcept {}
  void result() {
    if (exception) {
      std::rethrow_exception(exception);
    }
  }
};
} // namespace detail

template<typename T = void>
class [[nodiscard]] task {
public:
  using promise_type = detail::promise<T>;
  using value_type = T;

private:
  struct await_op_base {
    std::coroutine_handle<promise_type> this_coro;

    bool await_ready() const noexcept { return !this_coro || this_coro.done(); }
    std::coroutine_handle<> await_suspend(
      std::coroutine_handle<> awaiter) noexcept {
      this_coro.promise().continuation = awaiter;
      return this_coro;
    }
  };

public:
  [[nodiscard]] bool is_ready() const noexcept { return !ch_ || ch_.done(); }

  auto when_ready() noexcept {
    struct await_op : await_op_base {
      void await_resume() noexcept {}
    };
    return await_op{ch_};
  }

  auto operator co_await() const& noexcept {
    struct await_op : await_op_base {
      using await_op_base::this_coro;
      decltype(auto) await_resume() {
        if (!this_coro) {
          throw broken_promise{};
        }
        return this_coro.promise().result();
      }
    };
    return await_op{ch_};
  }

  auto operator co_await() const&& noexcept {
    struct await_op : await_op_base {
      using await_op_base::this_coro;
      decltype(auto) await_resume() {
        if (!this_coro) {
          throw broken_promise{};
        }
        return std::move(this_coro.promise()).result();
      }
    };
    return await_op{ch_};
  }

  explicit task(std::coroutine_handle<promise_type> handle) : ch_{handle} {}
  task(const task&) = delete;
  task& operator=(const task&) = delete;
  task(task&& other) noexcept : ch_{std::exchange(other.ch_, nullptr)} {}
  task& operator=(task&& other) noexcept {
    if (std::addressof(other) != this) {
      if (ch_) {
        ch_.destroy();
      }
      ch_ = std::exchange(other.ch_, nullptr);
    }
    return *this;
  }
  ~task() {
    if (ch_) {
      ch_.destroy();
    }
  }

private:
  std::coroutine_handle<promise_type> ch_;
};

template<typename T>
task<T> detail::promise<T>::get_return_object() noexcept {
  using handle_t = std::coroutine_handle<promise<T>>;
  return task<T>{handle_t::from_promise(*this)};
}

template<typename T>
task<T&> detail::promise<T&>::get_return_object() noexcept {
  using handle_t = std::coroutine_handle<promise<T&>>;
  return task<T&>{handle_t::from_promise(*this)};
}

task<void> detail::promise<void>::get_return_object() noexcept {
  using handle_t = std::coroutine_handle<promise<void>>;
  return task<void>{handle_t::from_promise(*this)};
}
} // namespace coro
