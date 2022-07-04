#include <gtest/gtest.h>

#include <math/math.hpp>

// NOLINTNEXTLINE
TEST(Math, FibonacciWorks) {
  using math::fib;
  EXPECT_EQ(fib(0), 0U);
  EXPECT_EQ(fib(1), 1U);
  EXPECT_EQ(fib(2), 1U);
  EXPECT_EQ(fib(3), 2U);
  EXPECT_EQ(fib(4), 3U);
  EXPECT_EQ(fib(5), 5U);
  EXPECT_EQ(fib(6), 8U);
  EXPECT_EQ(fib(7), 13U);
  EXPECT_EQ(fib(8), 21U);
  EXPECT_EQ(fib(9), 34U);
  EXPECT_EQ(fib(10), 55U);
  EXPECT_EQ(fib(11), 89U);
  EXPECT_EQ(fib(12), 144U);
  EXPECT_EQ(fib(13), 233U);
  EXPECT_EQ(fib(14), 377U);
  EXPECT_EQ(fib(15), 610U);
  EXPECT_EQ(fib(16), 987U);
  EXPECT_EQ(fib(17), 1597U);
  EXPECT_EQ(fib(18), 2584U);
  EXPECT_EQ(fib(19), 4181U);
  EXPECT_EQ(fib(20), 6765U);
  EXPECT_EQ(fib(21), 10946U);
  EXPECT_EQ(fib(22), 17711U);
  EXPECT_EQ(fib(23), 28657U);
  EXPECT_EQ(fib(24), 46368U);
  EXPECT_EQ(fib(25), 75025U);
  EXPECT_EQ(fib(26), 121393U);
  EXPECT_EQ(fib(27), 196418U);
  EXPECT_EQ(fib(28), 317811U);
  EXPECT_EQ(fib(29), 514229U);
  EXPECT_EQ(fib(30), 832040U);
  EXPECT_EQ(fib(31), 1346269U);
  EXPECT_EQ(fib(32), 2178309U);
  EXPECT_EQ(fib(33), 3524578U);
  EXPECT_EQ(fib(34), 5702887U);
  EXPECT_EQ(fib(35), 9227465U);
  EXPECT_EQ(fib(36), 14930352U);
  EXPECT_EQ(fib(37), 24157817U);
  EXPECT_EQ(fib(38), 39088169U);
  EXPECT_EQ(fib(39), 63245986U);
  EXPECT_EQ(fib(40), 102334155U);
  EXPECT_EQ(fib(41), 165580141U);
  EXPECT_EQ(fib(42), 267914296U);
  EXPECT_EQ(fib(43), 433494437U);
  EXPECT_EQ(fib(44), 701408733U);
  EXPECT_EQ(fib(45), 1134903170U);
  EXPECT_EQ(fib(46), 1836311903U);
  EXPECT_EQ(fib(47), 2971215073U);
  EXPECT_EQ(fib(48), 4807526976U);
  EXPECT_EQ(fib(49), 7778742049U);
}

// NOLINTNEXTLINE
TEST(Math, DifferentFibonacciVersionsProduceEqualResults) {
  namespace v1 = math::v1;
  namespace v2 = math::v2;
  constexpr auto iterations{30U};
  for (unsigned i{}; i < iterations; ++i) {
    EXPECT_EQ(v1::fib(i), v2::fib(i));
  }
}
