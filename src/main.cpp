#include <array>
#include <functional>
#include <iostream>
#include <math/math.hpp>
#include <numeric>

int main(int, char**) {
  constexpr std::array INDICES{3, 4, 5, 7};
  constexpr auto RESULT = std::transform_reduce(
    INDICES.cbegin(), INDICES.cend(), 0ULL, std::plus{}, math::fib);
  constexpr auto HELLO_MSG = std::to_array("Hello from C++");
  std::cout << HELLO_MSG.data() << RESULT << std::endl;
}
