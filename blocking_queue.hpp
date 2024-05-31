#pragma once

#include <cassert>
#include <cstdlib>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

namespace KTL {

template <typename T>
class UnboundedBlockingMPMCQueue {
public:
    void Put(T value) {
        std::lock_guard guard(mutex_);
        buffer_.push_back(std::move(value));
        not_empty_.notify_one();
    }

    T Take() {
        std::unique_lock lock(mutex_);
        while (buffer_.empty()) {
            not_empty_.wait(lock);
        }

        return takeLocked();
        /*while (true) {
            std::unique_lock guard(mutex_);
            if (!buffer_.empty()) {
                return takeLocked();
            } else {
                lock.unlock();
                std::this_thread::sleep_for(10ms);
            }
        }*/
    }

private:
    T takeLocked() {
        assert(!buffer_.empty());
        T front{std::move(buffer_.front())};
        buffer_.pop_front();
        return front;
    }

private:
    std::deque<T> buffer_;
    std::mutex mutex_;
    std::condition_variable not_empty_;
};

}