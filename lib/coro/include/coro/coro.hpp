#pragma once
#include <array>
#include <atomic>
#include <bit>
#include <concepts>
#include <condition_variable>
#include <coroutine>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>

namespace coro {
template<std::copyable T, std::size_t N>
  requires(std::popcount(N) == 1)
struct fifo {
  bool try_push(T value) {
    std::scoped_lock lk{m_};
    if (r_ - w_) {
      std::construct_at(reinterpret_cast<T*>(&ring_[w_++ & mask]),
                        std::move(value));
      read_cv_.notify_one();
      return true;
    }
    return false;
  }

  void push(T value) {
    std::unique_lock lk{m_};
    write_cv_.wait(lk, [this] { return r_ - w_; });
    std::construct_at(reinterpret_cast<T*>(&ring_[w_++ & mask]),
                      std::move(value));
    read_cv_.notify_one();
  }

  std::optional<T> try_pop() {
    std::scoped_lock lk{m_};
    if (r_ - w_ < N) {
      std::optional<T> ret{std::move(*reinterpret_cast<T*>(&ring_[r_ & mask]))};
      std::destroy_at(reinterpret_cast<T*>(&ring_[r_++ & mask]));
      write_cv_.notify_one();
      return ret;
    }
    return std::nullopt;
  }

  T pop() {
    std::unique_lock lk{m_};
    read_cv_.wait(lk, [this] { return r_ - w_ < N; });
    T ret{std::move(*reinterpret_cast<T*>(&ring_[r_ & mask]))};
    std::destroy_at(reinterpret_cast<T*>(&ring_[r_++ & mask]));
    write_cv_.notify_one();
    return ret;
  }

private:
  static constexpr auto mask = N - 1;
  struct storage_t {
    alignas(alignof(T)) std::array<std::byte, sizeof(T)> raw{};
  };
  std::mutex m_;
  std::condition_variable write_cv_;
  std::condition_variable read_cv_;
  std::uint64_t r_{N};
  std::uint64_t w_{};
  std::array<storage_t, N> ring_{};
};

template<std::invocable F,
         std::predicate P = decltype([] noexcept { return true; })>
  requires std::is_nothrow_invocable_v<F> && std::is_nothrow_invocable_v<P>
struct scoped_lambda {
  explicit scoped_lambda(F&& f, P&& p = P{})
    : f_{f}, p_{p}, cancelled_{false} {}
  scoped_lambda(const scoped_lambda&) = delete;
  scoped_lambda& operator=(const scoped_lambda&) = delete;
  scoped_lambda(scoped_lambda&& other)
    : f_{std::move(other.f_)}
    , p_{std::move(other.p_)}
    , cancelled_{std::exchange(other.cancelled_, true)} {}
  scoped_lambda& operator=(scoped_lambda&& other) {
    if (std::addressof(other) != this) {
      f_ = std::move(other.f_);
      p_ = std::move(other.p_);
      cancelled_ = std::exchange(other.cancelled_, true);
    }
    return *this;
  }
  ~scoped_lambda() {
    if (!cancelled_ && std::invoke(p_)) {
      std::invoke(f_);
    }
  }

  void cancel() noexcept { cancelled_ = true; }
  void operator()() noexcept {
    cancelled_ = true;
    std::invoke(f_);
  }

private:
  F f_;
  P p_;
  bool cancelled_;
};

template<std::invocable F>
auto on_scope_exit(F&& f) {
  return scoped_lambda(std::forward<F>(f));
}

template<std::invocable F>
auto on_scope_success(F&& f) {
  return scoped_lambda(
    std::forward<F>(f), [exceptions = std::uncaught_exceptions()] noexcept {
      return std::uncaught_exceptions() <= exceptions;
    });
}

template<std::invocable F>
auto on_scope_failure(F&& f) {
  return scoped_lambda(
    std::forward<F>(f), [exceptions = std::uncaught_exceptions()] noexcept {
      return std::uncaught_exceptions() > exceptions;
    });
}

template<typename T>
struct task;

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
  void unhandled_exception() { value = std::current_exception(); }
  template<std::convertible_to<T> V>
  void return_value(V&& v) noexcept(std::is_nothrow_convertible_v<V, T>) {
    value = std::forward<V>(v);
  }
  T& result() & {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return std::get<T>(value);
  }
  T&& result() && {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(std::move(value)));
    }
    return std::get<T>(std::move(value));
  }
  task<T> get_return_object() noexcept;
};

template<typename T>
struct promise<T&> : promise_base {
  std::variant<std::monostate, std::exception_ptr, T*> value;
  void unhandled_exception() { value = std::current_exception(); }
  void return_value(T& v) noexcept { value = std::addressof(v); }
  T& result() {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return *std::get<T*>(value);
  }
  task<T&> get_return_object() noexcept;
};

template<>
struct promise<void> : promise_base {
  std::exception_ptr exception;
  void unhandled_exception() { exception = std::current_exception(); }
  void return_void() const noexcept {}
  void result() {
    if (exception) {
      std::rethrow_exception(exception);
    }
  }
  task<void> get_return_object() noexcept;
};

class broken_promise : public std::logic_error {
public:
  broken_promise() : std::logic_error{"broken promise"} {}
};

template<typename T = void>
struct [[nodiscard]] task {
  using promise_type = promise<T>;
  using value_type = T;

  auto operator co_await() noexcept {
    struct awaitable {
      std::coroutine_handle<promise_type> h;
      bool await_ready() const noexcept { return !h || h.done(); }
      std::coroutine_handle<> await_suspend(
        std::coroutine_handle<> awaiter) noexcept {
        h.promise().continuation = awaiter;
        return h;
      }
      decltype(auto) await_resume() {
        if (!h) {
          throw broken_promise{};
        }
        return h.promise().result();
      }
    };
    return awaitable{h};
  }

  std::coroutine_handle<promise_type> h;

  [[nodiscard]] bool is_ready() const noexcept { return !h || h.done(); }

  explicit task(std::coroutine_handle<promise_type> handle) : h{handle} {}
  task(const task&) = delete;
  task& operator=(const task&) = delete;
  task(task&& other) noexcept : h{std::exchange(other.h, nullptr)} {}
  task& operator=(task&& other) noexcept {
    if (std::addressof(other) != this) {
      if (h) {
        h.destroy();
      }
      h = std::exchange(other.h, nullptr);
    }
    return *this;
  }
  ~task() {
    if (h) {
      h.destroy();
    }
  }
};

template<typename T>
task<T> promise<T>::get_return_object() noexcept {
  using handle_t = std::coroutine_handle<promise<T>>;
  return task<T>{handle_t::from_promise(*this)};
}

template<typename T>
task<T&> promise<T&>::get_return_object() noexcept {
  using handle_t = std::coroutine_handle<promise<T&>>;
  return task<T&>{handle_t::from_promise(*this)};
}

task<void> promise<void>::get_return_object() noexcept {
  using handle_t = std::coroutine_handle<promise<void>>;
  return task<void>{handle_t::from_promise(*this)};
}

struct io_service {
  struct schedule_op {
    friend struct io_service;
    explicit schedule_op(io_service& io) : io_{io} {}

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> awaiter) noexcept {
      awaiter_ = awaiter;
      io_.enqueue(this);
    }
    void await_resume() noexcept {}

  private:
    io_service& io_;
    std::coroutine_handle<> awaiter_{std::noop_coroutine()};
    schedule_op* next_{};
  };

  io_service() = default;
  io_service(const io_service&) = delete;
  io_service& operator=(const io_service&) = delete;
  io_service(io_service&&) = delete;
  io_service& operator=(io_service&&) = delete;
  ~io_service() = default;

  [[nodiscard]] schedule_op schedule() noexcept { return schedule_op{*this}; }

  bool process_one() {
#if 0
    auto* head = queue_.exchange(nullptr);
    while (head) {
      auto* next = head->next_;
      if (!fifo_.try_push(head->awaiter_.address())) {
        next = head;
        while (next->next_) {
          next = next->next_;
        }
        head = nullptr;
        while (!queue_.compare_exchange_weak(
          head, next, std::memory_order::release, std::memory_order::acquire)) {
          next->next_ = head;
        }
        break;
      }
      head = next;
    }
    return fifo_.try_pop()
      .transform([](void* addr) {
        std::coroutine_handle<>::from_address(addr).resume();
        return addr;
      })
      .value_or(nullptr);
#else
    auto* head = queue_.load(std::memory_order::acquire);
    while (head && !queue_.compare_exchange_weak(
                     head, head->next_, std::memory_order::release,
                     std::memory_order::acquire)) {}
    if (!head) {
      return false;
    }
    head->awaiter_.resume();
    return true;
#endif
  }

private:
  void enqueue(schedule_op* op) {
#if 0
    if (fifo_.try_push(op->awaiter_.address())) {
      return;
    }
#endif
    auto* head = queue_.load(std::memory_order::acquire);
    do {
      op->next_ = head;
    } while (!queue_.compare_exchange_weak(
      head, op, std::memory_order::release, std::memory_order::acquire));
  }

  std::atomic<schedule_op*> queue_;
  fifo<void*, 32> fifo_;
};

template<typename T>
struct is_coroutine_handle : std::false_type {};

template<typename P>
struct is_coroutine_handle<std::coroutine_handle<P>> : std::true_type {};

template<typename T>
constexpr auto is_coroutine_handle_v = is_coroutine_handle<T>::value;

template<typename T>
concept await_suspend_return_type =
  std::is_void_v<T> || std::is_same_v<T, bool> || is_coroutine_handle_v<T>;

template<typename T>
concept awaitable = requires(T t) {
  { t.await_ready() } -> std::same_as<bool>;
  {
    t.await_suspend(std::declval<std::coroutine_handle<>>())
    } -> await_suspend_return_type;
  t.await_resume();
};

auto get_awaiter(awaitable auto&& awaiter) noexcept {
  return std::move(awaiter);
}

auto get_awaiter(auto&& v) noexcept(noexcept(std::move(v).operator co_await()))
  -> decltype(std::move(v).operator co_await()) {
  return std::move(v).operator co_await();
}

auto get_awaiter(auto&& v) noexcept(noexcept(operator co_await(std::move(v))))
  -> decltype(operator co_await(std::move(v))) {
  return operator co_await(std::move(v));
}

template<typename Awaitable>
struct awaitable_traits {
  using awaiter_t = decltype(get_awaiter(std::declval<Awaitable>()));
  using await_result_t = decltype(std::declval<awaiter_t>().await_resume());
};

template<typename Scheduler, typename Awaitable>
task<std::conditional_t<
  std::is_reference_v<typename awaitable_traits<Awaitable>::await_result_t>,
  std::add_lvalue_reference_t<std::remove_reference_t<
    typename awaitable_traits<Awaitable>::await_result_t>>,
  typename awaitable_traits<Awaitable>::await_result_t>>
schedule_on(Scheduler& scheduler, Awaitable awaitable) {
  co_await scheduler.schedule();
  co_return co_await std::move(awaitable);
}

template<typename T>
struct sync_task;

template<typename T>
struct sync_promise final {
  using handle_t = std::coroutine_handle<sync_promise<T>>;
  auto get_return_object() noexcept { return handle_t::from_promise(*this); }
  std::suspend_always initial_suspend() const noexcept { return {}; }
  auto final_suspend() const noexcept {
    struct completion_notifier {
      bool await_ready() const noexcept { return false; }
      void await_suspend(handle_t h) const noexcept {
        h.promise().flag_->test_and_set();
        h.promise().flag_->notify_all();
      }
      void await_resume() const noexcept {}
    };
    return completion_notifier{};
  }
  template<std::convertible_to<T> V>
  auto yield_value(V&& v) noexcept(std::is_nothrow_convertible_v<V, T>) {
    value = v;
    return final_suspend();
  }
  [[noreturn]] void return_void() { std::unreachable(); }

  void unhandled_exception() { value = std::current_exception(); }

  T&& result() {
    if (std::holds_alternative<std::exception_ptr>(value)) {
      std::rethrow_exception(std::get<std::exception_ptr>(value));
    }
    return std::get<std::remove_reference_t<T>>(value);
  }

  void start(std::atomic_flag& flag) {
    flag_ = &flag;
    handle_t::from_promise(*this).resume();
  }
  std::atomic_flag* flag_;
  std::variant<std::monostate, std::remove_reference_t<T>, std::exception_ptr>
    value;
};

template<>
struct sync_promise<void> final {
  using handle_t = std::coroutine_handle<sync_promise<void>>;
  auto get_return_object() noexcept { return handle_t::from_promise(*this); }
  std::suspend_always initial_suspend() const noexcept { return {}; }
  auto final_suspend() const noexcept {
    struct completion_notifier {
      bool await_ready() const noexcept { return false; }
      void await_suspend(handle_t h) const noexcept {
        h.promise().flag_->test_and_set();
        h.promise().flag_->notify_all();
      }
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

  void start(std::atomic_flag& flag) {
    flag_ = &flag;
    handle_t::from_promise(*this).resume();
  }
  std::atomic_flag* flag_;
  std::exception_ptr exception;
};

template<typename T>
struct sync_task {
  using promise_type = sync_promise<T>;
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

  void start(std::atomic_flag& flag) noexcept {
    coroutine.promise().start(flag);
  }

  decltype(auto) result() { return coroutine.promise().result(); }

  handle_t coroutine;
};

template<typename Awaitable, typename Result = typename awaitable_traits<
                               Awaitable&&>::await_result_t>
sync_task<Result> make_sync_task(Awaitable&& awaitable) {
  co_yield co_await std::forward<Awaitable>(awaitable);
}

template<typename Awaitable,
         std::same_as<void> Result =
           typename awaitable_traits<Awaitable&&>::await_result_t>
sync_task<void> make_sync_task(Awaitable&& awaitable) {
  co_await std::forward<Awaitable>(awaitable);
}

template<typename Awaitable>
typename awaitable_traits<Awaitable&&>::await_result_t sync_wait(
  Awaitable&& awaiter) {
  std::atomic_flag flag;
  auto st = make_sync_task(std::forward<Awaitable>(awaiter));
  st.start(flag);
  flag.wait(false);
  return st.result();
}
} // namespace coro
