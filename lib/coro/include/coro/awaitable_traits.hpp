#pragma once
#include <coroutine>
#include <type_traits>
#include <utility>

namespace coro::detail {
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
concept awaitable_type = requires(T t) {
  { t.await_ready() } -> std::same_as<bool>;
  {
    t.await_suspend(std::declval<std::coroutine_handle<>>())
    } -> await_suspend_return_type;
  t.await_resume();
};

template<typename T>
concept has_co_await = requires(T t) {
  t.operator co_await();
};

template<typename T>
concept has_non_member_co_await = requires(T t) {
  operator co_await(t);
};

auto get_awaiter(awaitable_type auto&& awaiter) noexcept {
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
} // namespace coro::detail

namespace coro {
template<typename T>
concept awaitable = detail::awaitable_type<T> || detail::has_co_await<T> ||
  detail::has_non_member_co_await<T>;

template<awaitable T>
struct awaitable_traits {
  using awaiter_t = decltype(detail::get_awaiter(std::declval<T>()));
  using await_result_t = decltype(std::declval<awaiter_t>().await_resume());
};
} // namespace coro
