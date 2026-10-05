#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fmt/base.h>
#include <functional>
#include <math/math.hpp>
#include <numeric>

namespace {
void print_arg(const char* input) {
  char* buffer = static_cast<char*>(std::malloc(std::strlen(input))); // NOLINT
  std::strcpy(buffer, input);
  std::puts(buffer);
}
} // namespace

int main(int, char** argv) {
  print_arg(argv[0]); // NOLINT
  constexpr std::array indices{3, 4, 5, 7};
  constexpr auto result = std::transform_reduce(
    indices.cbegin(), indices.cend(), 0ULL, std::plus{}, math::fib);
  fmt::print("Hello from C++{}\n", result);
}
