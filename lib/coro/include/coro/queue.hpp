#pragma once
#include <array>
#include <bit>
#include <concepts>
#include <condition_variable>
#include <cstddef>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <utility>

namespace coro {
template<std::copyable T, std::size_t N>
  requires(std::popcount(N) == 1)
class queue {
public:
  [[nodiscard]] bool try_push(T value) {
    if (!m_.try_lock()) {
      return false;
    }
    std::scoped_lock lk{std::adopt_lock, m_};
    if (r_ - w_) {
      std::construct_at(
        reinterpret_cast<T*>(std::next(storage_.data(), w_++ & mask)),
        std::move(value));
      reader_.notify_one();
      return true;
    }
    return false;
  }

  void push(T value) {
    std::unique_lock lk{m_};
    writer_.wait(lk, [this] { return r_ - w_; });
    std::construct_at(
      reinterpret_cast<T*>(std::next(storage_.data(), w_++ & mask)),
      std::move(value));
    reader_.notify_one();
  }

  [[nodiscard]] std::optional<T> try_pop() {
    if (!m_.try_lock()) {
      return std::nullopt;
    }
    std::scoped_lock lk{std::adopt_lock, m_};
    if (r_ - w_ < N) {
      auto ptr = reinterpret_cast<T*>(std::next(storage_.data(), r_++ & mask));
      std::optional<T> ret{std::move(*ptr)};
      std::destroy_at(ptr);
      writer_.notify_one();
      return ret;
    }
    return std::nullopt;
  }

  [[nodiscard]] T pop() {
    std::unique_lock lk{m_};
    reader_.wait(lk, [this] { return r_ - w_ < N; });
    auto ptr = reinterpret_cast<T*>(std::next(storage_.data(), r_++ & mask));
    T ret{std::move(*ptr)};
    std::destroy_at(ptr);
    writer_.notify_one();
    return ret;
  }

private:
  static constexpr auto mask{N - 1};
  struct alignas(alignof(T)) obj_t {
    std::array<std::byte, sizeof(T)> raw;
  };
  std::array<obj_t, N> storage_;
  std::mutex m_;
  std::condition_variable reader_;
  std::condition_variable writer_;
  std::size_t r_{N};
  std::size_t w_{};
};
} // namespace coro
