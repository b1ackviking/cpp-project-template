#include <array>
#include <fmt/core.h>
#include <functional>
#include <math/math.hpp>
#include <numeric>

int main(int, char**) {
  constexpr std::array INDICES{3, 4, 5, 7};
  constexpr auto RESULT = std::transform_reduce(
    INDICES.cbegin(), INDICES.cend(), 0ULL, std::plus{}, math::fib);
  fmt::print("Hello from C++{}\n", RESULT);
}
