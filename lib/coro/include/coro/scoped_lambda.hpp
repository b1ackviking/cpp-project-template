#pragma once
#include <concepts>
#include <exception>
#include <functional>
#include <type_traits>
#include <utility>

namespace coro {
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
} // namespace coro
