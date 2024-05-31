#pragma once

#include <cstddef>
#include <functional>
#include <vector>
#include <thread>

#include "blocking_queue.hpp"

namespace KTL {

class ThreadPool {
 public:
  explicit ThreadPool(size_t workers);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Schedules task for execution in one of the worker threads
  void Submit(std::function<void()> task);

  // Waits until outstanding work count has reached zero
  void WaitIdle();

  // Stops the worker threads as soon as possible
  // Pending tasks will be discarded
  void Stop();

  // Locates current thread pool from worker thread
  static ThreadPool* Current();

 private:
  void StartWorkingThreads(size_t count);
  void WorkerRoutine();

 private:
  std::vector<std::thread> workers_;
  UnboundedBlockingMPMCQueue<std::function<void()>> tasks_;
};

inline ThreadPool* Current() {
  return ThreadPool::Current();
}

}