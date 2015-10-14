#ifndef THREADPOOL_H
#define THREADPOOL_H 1

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

#include <dsnutil/dsnutil_cpp_Export.h>
#include <dsnutil/exception.h>

namespace dsn {

/// \brief Fixed size thread pool
///
/// This can be used to quickly implement thread pool of fixed size where arbitrary tasks
/// can be enqueued and executed in parallel.
///
/// The pool is designed so that it can take an arbitrary number of tasks but only executes
/// a given number of them in parallel.
///
/// Enqueuing a tasks will return a std::future so the queued tasks can return values.
class dsnutil_cpp_EXPORT ThreadPool {
public:
    ThreadPool(size_t size = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        typedef typename std::result_of<F(Args...)>::type return_type;

        if (m_stop)
            DSN_DEFAULT_EXCEPTION_SIMPLE("Cannot enqueue tasks on stopped ThreadPool!");

        auto task = std::make_shared<std::packaged_task<return_type()> >(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.push([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    void stop();

private:
    /// \brief Currently active worker threads to execute tasks
    std::vector<std::thread> workers;

    /// \brief Tasks queued on the pool
    std::queue<std::function<void()> > tasks;

    /// \brief Mutex for synchronized access to task queue
    std::mutex queue_mutex;

    /// \brief Condition variable for state change notifications
    std::condition_variable condition;

    /// \brief Flag to indicate wether pool execution shall be stopped
    bool m_stop{ false };
};
}

#endif
