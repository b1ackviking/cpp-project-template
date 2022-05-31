#pragma once
#include <algorithm>
#include <atomic>
#include <chrono>
#include <coroutine>
#include <cstddef>
#include <functional>
#include <optional>
#include <vector>

namespace coro {
template<template<typename, std::size_t> typename Fifo>
class io_service {
  class schedule_op {
    friend class io_service;

  public:
    explicit schedule_op(io_service& io) : io_{io} {}

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) noexcept {
      ch_ = h;
      io_.enqueue(io_.insert_ops_queue_, this);
    }
    void await_resume() noexcept {}

  private:
    io_service& io_;
    std::coroutine_handle<> ch_{std::noop_coroutine()};
    schedule_op* next_{};
  };

  class timed_schedule_op {
    friend class io_service;

  public:
    timed_schedule_op(
      io_service& io,
      std::chrono::high_resolution_clock::time_point resume_time)
      : op_{io}, resume_time_{resume_time} {}

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) noexcept {
      op_.ch_ = h;
      op_.io_.enqueue(op_.io_.insert_timed_ops_queue_, this);
      if (ref_count_.fetch_sub(1, std::memory_order::acquire) == 0) {
        op_.io_.enqueue(op_.io_.insert_ops_queue_, &op_);
      }
    }
    void await_resume() const noexcept {}

  private:
    schedule_op op_;
    std::chrono::high_resolution_clock::time_point resume_time_;
    timed_schedule_op* next_{};
    std::atomic_uint32_t ref_count_{2};
  };

public:
  io_service() = default;
  io_service(const io_service&) = delete;
  io_service& operator=(const io_service&) = delete;
  io_service(io_service&&) = delete;
  io_service& operator=(io_service&&) = delete;
  ~io_service() = default;

  [[nodiscard]] schedule_op schedule() noexcept { return schedule_op{*this}; }

  [[nodiscard]] timed_schedule_op schedule_at(
    std::chrono::high_resolution_clock::time_point time) noexcept {
    return timed_schedule_op{*this, time};
  }

  template<typename Rep, typename Period>
  [[nodiscard]] timed_schedule_op schedule_after(
    std::chrono::duration<Rep, Period> duration) noexcept {
    return timed_schedule_op{
      *this, std::chrono::high_resolution_clock::now() + duration};
  }

  bool process_one() {
    auto* head = insert_ops_queue_.exchange(nullptr);
    while (head) {
      auto* next = head->next_;
      if (!work_queue_.try_push(head)) {
        next = head;
        while (next->next_) {
          next = next->next_;
        }
        head = nullptr;
        while (!insert_ops_queue_.compare_exchange_weak(
          head, next, std::memory_order::release, std::memory_order::acquire)) {
          next->next_ = head;
        }
        break;
      }
      head = next;
    }
    return work_queue_.try_pop()
      .transform([](schedule_op* op) {
        op->ch_.resume();
        return op;
      })
      .value_or(nullptr);
  }

  void process_schedule() {
    auto* head = insert_timed_ops_queue_.exchange(nullptr);
    while (head) {
      wait_queue_.push_back(head);
      std::ranges::push_heap(
        wait_queue_, std::greater{},
        [](timed_schedule_op* op) { return op->resume_time_; });
      head = head->next_;
    }
    const auto current_time = std::chrono::high_resolution_clock::now();
    while (!wait_queue_.empty() &&
           wait_queue_.front()->resume_time_ <= current_time) {
      enqueue(insert_ops_queue_, &wait_queue_.front()->op_);
      std::ranges::pop_heap(
        wait_queue_, std::greater{},
        [](timed_schedule_op* op) { return op->resume_time_; });
      wait_queue_.pop_back();
    }
  }

private:
  void enqueue(std::atomic<timed_schedule_op*>& queue,
               timed_schedule_op* op) noexcept {
    auto* head = queue.load(std::memory_order::acquire);
    do {
      op->next_ = head;
    } while (!queue.compare_exchange_weak(
      head, op, std::memory_order::release, std::memory_order::acquire));
  }
  void enqueue(std::atomic<schedule_op*>& queue, schedule_op* op) noexcept {
    if (work_queue_.try_push(op)) {
      return;
    }
    auto* head = queue.load(std::memory_order::acquire);
    do {
      op->next_ = head;
    } while (!queue.compare_exchange_weak(
      head, op, std::memory_order::release, std::memory_order::acquire));
  }

  std::atomic<schedule_op*> insert_ops_queue_{};
  std::atomic<timed_schedule_op*> insert_timed_ops_queue_{};
  Fifo<schedule_op*, 32> work_queue_{};
  std::vector<timed_schedule_op*> wait_queue_{};
};
} // namespace coro
