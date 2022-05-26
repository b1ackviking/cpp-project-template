#include <array>
#include <coro/coro.hpp>
#include <fmt/core.h>
#include <functional>
#include <math/math.hpp>
#include <numeric>
#include <utility>

coro::task<> bar() {
  fmt::print("bar 1\n");
  fmt::print("bar 2\n");
  co_return;
}

coro::task<int> baz() {
  fmt::print("baz 1\n");
  fmt::print("baz 2\n");
  co_return 42;
}

coro::task<int> foo(coro::io_service& io) {
  fmt::print("foo 1\n");
  fmt::print("foo 2\n");
  co_await coro::schedule_on(io, bar());
  co_await io.schedule();
  fmt::print("foo 3\n");
  co_return co_await baz();
}

#if 0
coro::async_generator<float> adc() {
  while (true) {
    co_yield co_await read_voltage();
  }
}

void sm() {
  while (true) {
    switch (auto event = co_await next_event(); event.type) {
    case adc_reading: break;
    case can_message: break;
    }
  }
}
#endif

int main(int, char**) {
  auto sayHello = coro::on_scope_success([] noexcept {
    constexpr std::array indices{3, 4, 5, 7};
    constexpr auto result = std::transform_reduce(
      indices.cbegin(), indices.cend(), 0ULL, std::plus{}, math::fib);
    try {
      fmt::print("Hello from C++{}\n", result);
    } catch (...) {}
  });

  coro::io_service io;
  coro::sync_wait(bar());
  auto b = bar();
  auto f = foo(io);

  auto s = coro::schedule_on(io, std::move(b));
  s.h.resume();

  f.h.resume();
  while (io.process_one()) {
    fmt::print("resume\n");
  }
  if (f.is_ready()) {
    fmt::print("f = {}\n", f.h.promise().result());
  }
}
