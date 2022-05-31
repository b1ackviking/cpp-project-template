#include <array>
#include <atomic>
#include <chrono>
// #include <coro/coro.hpp>
#include <coro/io_service.hpp>
#include <coro/queue.hpp>
#include <coro/sync_wait.hpp>
#include <coro/task.hpp>
#include <coro/when_all_ready.hpp>
#include <cstddef>
#include <fmt/core.h>
#include <functional>
#include <math/math.hpp>
#include <numeric>
#include <thread>
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

coro::task<int> foo(auto& io) {
  fmt::print("foo 1\n");
  fmt::print("foo 2\n");
  // co_await coro::schedule_on(io, bar());
  co_await io.schedule_after(std::chrono::seconds{1});
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

coro::task<> run_bar() {
  co_await bar();
}

coro::task<int> run_foo(auto& io) {
  co_return co_await foo(io);
}

coro::task<> worker(auto& io) {
  while (true) {
    io.process_schedule();
    io.process_one();
  }
  co_return;
}

int main(int, char**) {
  coro::io_service<coro::queue> io;

  struct event_t {
    std::atomic_flag f;
    void wait() noexcept { f.wait(false); }
    void set() noexcept {
      f.test_and_set();
      f.notify_all();
    }
  } e, e2;

#if 0
  auto [b1, b2, b3] =
    coro::sync_wait(coro::when_all_ready(bar(), bar(), baz()), e);
  auto b = bar();
  auto f = foo(io);
  fmt::print("{}\n", b3.result());

  // auto s = coro::schedule_on(io, std::move(b));
  // s.h.resume();
  std::jthread worker{[&io](std::stop_token st) {
    while (!st.stop_requested()) {
      io.process_schedule();
      io.process_one();
    }
  }};
#endif
  coro::sync_wait(coro::when_all_ready(foo(io), bar(), worker(io)), e2);
}
