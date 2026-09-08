#pragma once

#include <algorithm>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <exception>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace snapecs::detail {

// thread pool
class JThreadPool {
public:
  JThreadPool() = default;

  explicit JThreadPool(std::size_t workerCount) {
    configure(workerCount);
  }

  ~JThreadPool() {
    stop();
  }

  // 복사 금지
  JThreadPool(const JThreadPool&) = delete;
  JThreadPool& operator=(const JThreadPool&) = delete;

  void configure(std::size_t workerCount) {
    // 기존 worker가 있다면 정지
    stop();

    workerCount_ = workerCount;

    // worker vector 생성
    workers_.reserve(workerCount_);
    for (std::size_t index = 0; index < workerCount_; ++index) {
      // jthread 함수는 stop_token을 받을 수 있습니다.
      workers_.emplace_back([this](std::stop_token stopToken) { worker_loop(stopToken); });
    }
  }

  void stop() {
    for (auto& worker : workers_) {
      worker.request_stop();
    }

    // 대기 중인 worker들이 stop 요청을 확인하도록 깨웁니다.
    queueCv_.notify_all();
    workers_.clear();

    // 남은 task를 버리고 worker 수를 0으로 초기화합니다.
    std::lock_guard lock(queueMutex_);
    tasks_.clear();
    workerCount_ = 0;
  }

  std::size_t worker_count() const {
    return workerCount_;
  }

  template <typename Fn>
  void run_batch(std::vector<Fn>& tasks) {
    if (tasks.empty()) {
      return;
    }

    // worker가 없거나 worker thread 내부에서 다시 batch를 실행하려는 경우 순차 실행합니다.
    // 이렇게 하면 worker가 자기 자신이 기다리는 작업을 queue에 넣고 deadlock 나는 상황을 피합니다.
    if (workerCount_ == 0 || is_worker_thread()) {
      for (auto& task : tasks) {
        task();
      }
      return;
    }

    std::mutex doneMutex;
    std::condition_variable doneCv;
    std::mutex exceptionMutex;
    std::exception_ptr firstException;
    std::size_t remaining = tasks.size();

    for (auto& task : tasks) {
      enqueue([&, task = std::move(task)]() mutable {
        try {
          task();
        } catch (...) {
          // 여러 task가 예외를 던져도 첫 예외 하나만 저장해 호출 thread에서 다시 던집니다.
          std::lock_guard exceptionLock(exceptionMutex);
          if (firstException == nullptr) {
            firstException = std::current_exception();
          }
        }

        {
          std::lock_guard doneLock(doneMutex);
          --remaining;

          if (remaining == 0) {
            doneCv.notify_one();
          }
        }
      });
    }

    {
      std::unique_lock doneLock(doneMutex);
      doneCv.wait(doneLock, [&] { return remaining == 0; });
    }

    if (firstException != nullptr) {
      std::rethrow_exception(firstException);
    }
  }

private:
  // thread_local이므로 각 OS thread마다 currentPool_ 값이 따로 있습니다.
  // worker_loop 안에서 this로 세팅해 "현재 thread가 이 pool의 worker인지" 판별합니다.
  static thread_local const JThreadPool* currentPool_;

  bool is_worker_thread() const {
    return currentPool_ == this;
  }

  void enqueue(std::function<void()> task) {
    {
      std::lock_guard lock(queueMutex_);
      tasks_.push_back(std::move(task));
    }
    queueCv_.notify_one();
  }

  void worker_loop(std::stop_token stopToken) {
    currentPool_ = this;

    while (true) {
      std::function<void()> task;
      {
        std::unique_lock lock(queueMutex_);

        // condition_variable_any는 stop_token을 받는 wait overload를 지원합니다.
        // task가 들어오거나 stop 요청이 오면 깨어납니다.
        queueCv_.wait(lock, stopToken, [&] { return !tasks_.empty(); });

        if (stopToken.stop_requested() && tasks_.empty()) {
          break;
        }

        if (tasks_.empty()) {
          continue;
        }

        task = std::move(tasks_.front());
        tasks_.pop_front();
      }

      task();
    }

    currentPool_ = nullptr;
  }

  std::size_t workerCount_ = 0;
  std::vector<std::jthread> workers_;
  std::deque<std::function<void()>> tasks_;
  std::mutex queueMutex_;
  std::condition_variable_any queueCv_;
};

inline thread_local const JThreadPool* JThreadPool::currentPool_ = nullptr;

// workerCount 설정이 0이면 하드웨어 동시성에서 main thread 하나를 뺀 값을 사용합니다.
// hardware_concurrency가 0 또는 1이면 최소 1 worker를 반환합니다.
inline std::size_t resolve_worker_count(std::size_t requestedWorkerCount) {
  if (requestedWorkerCount != 0) {
    return requestedWorkerCount;
  }

  const auto hardwareCount = std::thread::hardware_concurrency();
  if (hardwareCount <= 1) {
    return 1;
  }

  return std::max<std::size_t>(1, static_cast<std::size_t>(hardwareCount) - 1);
}

} // namespace snapecs::detail
