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
  EXPECT_EQ(fib(17), 1'597U);
  EXPECT_EQ(fib(18), 2'584U);
  EXPECT_EQ(fib(19), 4'181U);
  EXPECT_EQ(fib(20), 6'765U);
  EXPECT_EQ(fib(21), 10'946U);
  EXPECT_EQ(fib(22), 17'711U);
  EXPECT_EQ(fib(23), 28'657U);
  EXPECT_EQ(fib(24), 46'368U);
  EXPECT_EQ(fib(25), 75'025U);
  EXPECT_EQ(fib(26), 121'393U);
  EXPECT_EQ(fib(27), 196'418U);
  EXPECT_EQ(fib(28), 317'811U);
  EXPECT_EQ(fib(29), 514'229U);
  EXPECT_EQ(fib(30), 832'040U);
  EXPECT_EQ(fib(31), 1'346'269U);
  EXPECT_EQ(fib(32), 2'178'309U);
  EXPECT_EQ(fib(33), 3'524'578U);
  EXPECT_EQ(fib(34), 5'702'887U);
  EXPECT_EQ(fib(35), 9'227'465U);
  EXPECT_EQ(fib(36), 14'930'352U);
  EXPECT_EQ(fib(37), 24'157'817U);
  EXPECT_EQ(fib(38), 39'088'169U);
  EXPECT_EQ(fib(39), 63'245'986U);
  EXPECT_EQ(fib(40), 102'334'155U);
  EXPECT_EQ(fib(41), 165'580'141U);
  EXPECT_EQ(fib(42), 267'914'296U);
  EXPECT_EQ(fib(43), 433'494'437U);
  EXPECT_EQ(fib(44), 701'408'733U);
  EXPECT_EQ(fib(45), 1'134'903'170U);
  EXPECT_EQ(fib(46), 1'836'311'903U);
  EXPECT_EQ(fib(47), 2'971'215'073U);
  EXPECT_EQ(fib(48), 4'807'526'976U);
  EXPECT_EQ(fib(49), 7'778'742'049U);
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
