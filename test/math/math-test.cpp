#include <gtest/gtest.h>

#include <math/math.hpp>

TEST(Math, FibonacciWorks) {
  using math::fib;
  EXPECT_EQ(fib(0), 0u);
  EXPECT_EQ(fib(1), 1u);
  EXPECT_EQ(fib(2), 1u);
  EXPECT_EQ(fib(3), 2u);
  EXPECT_EQ(fib(4), 3u);
  EXPECT_EQ(fib(5), 5u);
  EXPECT_EQ(fib(6), 8u);
  EXPECT_EQ(fib(7), 13u);
  EXPECT_EQ(fib(8), 21u);
  EXPECT_EQ(fib(9), 34u);
  EXPECT_EQ(fib(10), 55u);
  EXPECT_EQ(fib(11), 89u);
  EXPECT_EQ(fib(12), 144u);
  EXPECT_EQ(fib(13), 233u);
  EXPECT_EQ(fib(14), 377u);
  EXPECT_EQ(fib(15), 610u);
  EXPECT_EQ(fib(16), 987u);
  EXPECT_EQ(fib(17), 1597u);
  EXPECT_EQ(fib(18), 2584u);
  EXPECT_EQ(fib(19), 4181u);
  EXPECT_EQ(fib(20), 6765u);
  EXPECT_EQ(fib(21), 10946u);
  EXPECT_EQ(fib(22), 17711u);
  EXPECT_EQ(fib(23), 28657u);
  EXPECT_EQ(fib(24), 46368u);
  EXPECT_EQ(fib(25), 75025u);
  EXPECT_EQ(fib(26), 121393u);
  EXPECT_EQ(fib(27), 196418u);
  EXPECT_EQ(fib(28), 317811u);
  EXPECT_EQ(fib(29), 514229u);
  EXPECT_EQ(fib(30), 832040u);
  EXPECT_EQ(fib(31), 1346269u);
  EXPECT_EQ(fib(32), 2178309u);
  EXPECT_EQ(fib(33), 3524578u);
  EXPECT_EQ(fib(34), 5702887u);
  EXPECT_EQ(fib(35), 9227465u);
  EXPECT_EQ(fib(36), 14930352u);
  EXPECT_EQ(fib(37), 24157817u);
  EXPECT_EQ(fib(38), 39088169u);
  EXPECT_EQ(fib(39), 63245986u);
  EXPECT_EQ(fib(40), 102334155u);
  EXPECT_EQ(fib(41), 165580141u);
  EXPECT_EQ(fib(42), 267914296u);
  EXPECT_EQ(fib(43), 433494437u);
  EXPECT_EQ(fib(44), 701408733u);
  EXPECT_EQ(fib(45), 1134903170u);
  EXPECT_EQ(fib(46), 1836311903u);
  EXPECT_EQ(fib(47), 2971215073u);
  EXPECT_EQ(fib(48), 4807526976u);
  EXPECT_EQ(fib(49), 7778742049u);
}

TEST(Math, DifferentFibonacciVersionsProduceEqualResults) {
  using namespace math;
  for (unsigned long long i{}; i < 50; ++i)
    EXPECT_EQ(v1::fib(i), v2::fib(i));
}
