#include <array>
#include <fmt/core.h>
#include <functional>
#include <math/math.hpp>
#include <numeric>

int main(int, char**) {
  constexpr std::array indices{3, 4, 5, 7};
  constexpr auto result = std::transform_reduce(
    indices.cbegin(), indices.cend(), 0ULL, std::plus{}, math::fib);
  fmt::print("Hello from C++{}\n", result);
}
