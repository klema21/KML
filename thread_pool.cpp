#include <iostream>

#include "thread_pool.hpp"


namespace KTL {

ThreadPool::ThreadPool(size_t workers) {
    StartWorkingThreads(workers);
}

ThreadPool::~ThreadPool() {
  assert(workers_.empty());
}

void ThreadPool::Submit(std::function<void()> task) {
  tasks_.Put(std::move(task));
}

void ThreadPool::WaitIdle() {
  std::this_thread::sleep_for(3s);
}

void ThreadPool::Stop() {
  // Not implemented
}

void ThreadPool::StartWorkingThreads(size_t count) {
    for (int i = 0; i < count; ++i) {
        workers_.emplace_back([this](){
            WorkerRoutine();
        });
    }
}

void ThreadPool::WorkerRoutine() {
  while (true) {
    auto task = tasks_.Take();
    task();
  }
}

ThreadPool* ThreadPool::Current() {
  //return pool; ??
}

} 