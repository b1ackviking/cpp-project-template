#include <gtest/gtest.h>

#include <math/math.hpp>

TEST(Math, Fibonacci) {
  using math::fib;
  EXPECT_EQ(fib(0), 0);
  EXPECT_EQ(fib(1), 1);
  EXPECT_EQ(fib(2), 1);
  EXPECT_EQ(fib(3), 2);
  EXPECT_EQ(fib(4), 3);
  EXPECT_EQ(fib(5), 5);
  EXPECT_EQ(fib(6), 8);
  EXPECT_EQ(fib(7), 13);
  EXPECT_EQ(fib(8), 21);
  EXPECT_EQ(fib(9), 34);
  EXPECT_EQ(fib(10), 55);
  EXPECT_EQ(fib(11), 89);
  EXPECT_EQ(fib(12), 144);
  EXPECT_EQ(fib(13), 233);
  EXPECT_EQ(fib(14), 377);
  EXPECT_EQ(fib(15), 610);
  EXPECT_EQ(fib(16), 987);
  EXPECT_EQ(fib(17), 1597);
  EXPECT_EQ(fib(18), 2584);
  EXPECT_EQ(fib(19), 4181);
  EXPECT_EQ(fib(20), 6765);
  EXPECT_EQ(fib(21), 10946);
  EXPECT_EQ(fib(22), 17711);
  EXPECT_EQ(fib(23), 28657);
  EXPECT_EQ(fib(24), 46368);
  EXPECT_EQ(fib(25), 75025);
  EXPECT_EQ(fib(26), 121393);
  EXPECT_EQ(fib(27), 196418);
  EXPECT_EQ(fib(28), 317811);
  EXPECT_EQ(fib(29), 514229);
  EXPECT_EQ(fib(30), 832040);
  EXPECT_EQ(fib(31), 1346269);
  EXPECT_EQ(fib(32), 2178309);
  EXPECT_EQ(fib(33), 3524578);
  EXPECT_EQ(fib(34), 5702887);
  EXPECT_EQ(fib(35), 9227465);
  EXPECT_EQ(fib(36), 14930352);
  EXPECT_EQ(fib(37), 24157817);
  EXPECT_EQ(fib(38), 39088169);
  EXPECT_EQ(fib(39), 63245986);
  EXPECT_EQ(fib(40), 102334155);
  EXPECT_EQ(fib(41), 165580141);
  EXPECT_EQ(fib(42), 267914296);
  EXPECT_EQ(fib(43), 433494437);
  EXPECT_EQ(fib(44), 701408733);
  EXPECT_EQ(fib(45), 1134903170);
  EXPECT_EQ(fib(46), 1836311903);
  EXPECT_EQ(fib(47), 2971215073);
}
