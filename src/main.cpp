#include <array>
#include <iostream>
#include <math/math.hpp>

int main(int, char**) {
  using math::fib;
  constexpr auto HELLO_MSG = std::to_array("Hello from C++");
  std::cout << HELLO_MSG.data() << fib(3) + fib(5) + fib(7) << std::endl;
}
