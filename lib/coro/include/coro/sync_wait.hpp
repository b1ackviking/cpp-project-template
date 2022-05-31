#pragma once
#include "coro/awaitable_traits.hpp"

#include <atomic>
#include <concepts>
#include <coroutine>
#include <exception>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

namespace coro::detail {
template<typename T>
concept signalable = requires(T t) {
  t.wait();
  t.set();
};

template<typename T, signalable Event>
struct sync_task;

template<typename T, signalable Event>
struct sync_promise final {
  using handle_t = std::coroutine_handle<sync_promise<T, Event>>;
  auto get_return_object() noexcept { return handle_t::from_promise(*this); }
  std::suspend_always initial_suspend() const noexcept { return {}; }
  auto final_suspend() const noexcept {
    struct completion_notifier {
      bool await_ready() const noexcept { return false; }
      void await_suspend(handle_t h) const noexcept { h.promise().e_->set(); }
      void await_resume() const noexcept {}
    };
    return completion_notifier{};
  }
  template<std::convertible_to<T> V>
  auto yield_value(V&& v) noexcept(std::is_nothrow_convertible_v<V, T>) {
    value = std::move(v);
    return final_suspend();
  }
  [[noreturn]] void return_void() { std::unreachable(); }

  void unhandled_exception() { value = std::current_exception(); }

  [[nodiscard]] T&& result() {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return std::get<std::remove_reference_t<T>>(std::move(value));
  }

  void start(Event& e) {
    e_ = &e;
    handle_t::from_promise(*this).resume();
  }
  Event* e_;
  std::variant<std::monostate, std::remove_reference_t<T>, std::exception_ptr>
    value;
};

template<signalable Event>
struct sync_promise<void, Event> final {
  using handle_t = std::coroutine_handle<sync_promise<void, Event>>;
  auto get_return_object() noexcept { return handle_t::from_promise(*this); }
  std::suspend_always initial_suspend() const noexcept { return {}; }
  auto final_suspend() const noexcept {
    struct completion_notifier {
      bool await_ready() const noexcept { return false; }
      void await_suspend(handle_t h) const noexcept { h.promise().e_->set(); }
      void await_resume() const noexcept {}
    };
    return completion_notifier{};
  }
  void return_void() {}

  void unhandled_exception() { exception = std::current_exception(); }

  void result() {
    if (exception) {
      std::rethrow_exception(exception);
    }
  }

  void start(Event& e) {
    e_ = &e;
    handle_t::from_promise(*this).resume();
  }
  Event* e_;
  std::exception_ptr exception;
};

template<typename T, signalable Event>
struct sync_task {
  using promise_type = sync_promise<T, Event>;
  using handle_t = std::coroutine_handle<promise_type>;

  sync_task(handle_t h) noexcept : coroutine{h} {}
  sync_task(const sync_task&) = delete;
  sync_task& operator=(const sync_task&) = delete;
  sync_task(sync_task&& other) noexcept
    : coroutine(std::exchange(other.coroutine, nullptr)) {}
  sync_task& operator=(sync_task&& other) {
    if (std::addressof(other) != this) {
      if (coroutine) {
        coroutine.destroy();
      }
      coroutine = std::exchange(other.coroutine, nullptr);
    }
    return *this;
  }
  ~sync_task() {
    if (coroutine) {
      coroutine.destroy();
    }
  }

  void start(Event& e) noexcept { coroutine.promise().start(e); }

  decltype(auto) result() { return std::move(coroutine.promise()).result(); }

  handle_t coroutine;
};

template<awaitable T, signalable Event,
         typename Result = typename awaitable_traits<T&&>::await_result_t>
sync_task<Result, Event> make_sync_task(T&& awaitable, Event&) {
  co_yield co_await std::forward<T>(awaitable);
}

template<
  awaitable T, signalable Event,
  std::same_as<void> Result = typename awaitable_traits<T&&>::await_result_t>
sync_task<Result, Event> make_sync_task(T&& awaitable, Event&) {
  co_await std::forward<T>(awaitable);
}
} // namespace coro::detail

namespace coro {
template<awaitable T, detail::signalable Event>
std::remove_reference_t<typename awaitable_traits<T&&>::await_result_t>
sync_wait(T&& awaiter, Event& e) {
  auto st = detail::make_sync_task(std::forward<T>(awaiter), e);
  st.start(e);
  e.wait();
  return st.result();
}
} // namespace coro
