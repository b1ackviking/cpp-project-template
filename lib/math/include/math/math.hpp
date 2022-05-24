#pragma once

namespace math {
#ifdef RECURSIVE_FIB
inline
#endif
  namespace v1 {
constexpr auto fib(unsigned long long n) {
  if (n < 2)
    return n;
  return fib(n - 1) + fib(n - 2);
}
} // namespace v1

#ifndef RECURSIVE_FIB
inline
#endif
  namespace v2 {
constexpr auto fib(unsigned long long n) {
  struct V {
    unsigned long long v[2]{};
  };
  struct M {
    unsigned long long m[2][2]{};
    constexpr M operator*(const M& r) const noexcept {
      return M{{
        {m[0][0] * r.m[0][0] + m[0][1] * r.m[1][0],
         m[0][0] * r.m[0][1] + m[0][1] * r.m[1][1]},
        {m[1][0] * r.m[0][0] + m[1][1] * r.m[1][0],
         m[1][0] * r.m[0][1] + m[1][1] * r.m[1][1]},
      }};
    }
    constexpr M& operator*=(const M& r) noexcept {
      *this = *this * r;
      return *this;
    }
    constexpr V operator*(const V& r) const noexcept {
      return V{{
        m[0][0] * r.v[0] + m[0][1] * r.v[1],
        m[1][0] * r.v[0] + m[1][1] * r.v[1],
      }};
    }
  };

  V v{{0, 1}};
  for (M i{{
         {0, 1},
         {1, 1},
       }};
       n; n >>= 1, i *= i)
    if (n & 1)
      v = i * v;
  return v.v[0];
}
} // namespace v2
} // namespace math
