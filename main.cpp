#include <array>
#include <iostream>

int main()
{
  constexpr auto HELLO_MSG = std::to_array("Hello from C++20\n");
  std::cout << HELLO_MSG.data() << std::endl;
}
