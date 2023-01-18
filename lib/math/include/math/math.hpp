#pragma once
#include <cstdint>

namespace math {
#ifdef RECURSIVE_FIB
inline
#endif
  namespace v1 {
// NOLINTNEXTLINE
constexpr auto fib(std::uint64_t n) {
  if (n < 2) {
    return n;
  }
  return fib(n - 1) + fib(n - 2);
}
} // namespace v1

#ifndef RECURSIVE_FIB
inline
#endif
  namespace v2 {
constexpr auto fib(std::uint64_t n) {
  struct vec {
    std::uint64_t v[2]{}; // NOLINT
  };

  struct mat {
    std::uint64_t m[2][2]{}; // NOLINT

    constexpr mat operator*(const mat& r) const noexcept {
      return mat{{
        {m[0][0] * r.m[0][0] + m[0][1] * r.m[1][0],
         m[0][0] * r.m[0][1] + m[0][1] * r.m[1][1]},
        {m[1][0] * r.m[0][0] + m[1][1] * r.m[1][0],
         m[1][0] * r.m[0][1] + m[1][1] * r.m[1][1]},
      }};
    }

    constexpr mat& operator*=(const mat& r) noexcept {
      *this = *this * r;
      return *this;
    }

    constexpr vec operator*(const vec& r) const noexcept {
      return vec{{
        m[0][0] * r.v[0] + m[0][1] * r.v[1],
        m[1][0] * r.v[0] + m[1][1] * r.v[1],
      }};
    }
  };

  vec v{{0, 1}};
  for (mat i{{
         {0, 1},
         {1, 1},
       }};
       n; n >>= 1U, i *= i) {
    if (n & 1U) {
      v = i * v;
    }
  }
  return v.v[0];
}
} // namespace v2
} // namespace math
