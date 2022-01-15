#include <array>
#include <iostream>

int main(int, char**) {
  constexpr auto HELLO_MSG = std::to_array("Hello from C++20");
  std::cout << HELLO_MSG.data() << std::endl;
}
