#ifndef THREADPOOL_H
#define THREADPOOL_H 1

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

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

    /// \brief Enqueue a task on this thread pool
    ///
    /// Queues a task for execution in the thread pool. The task will appended to \p tasks and executed by
    /// one of the threads in \p workers at some point in the future.
    ///
    /// \param f Function that shall be executed in the thread pool (this can be anything callable)
    /// \param args Variable arguments to \a f
    ///
    /// \return \p std::future<> with the result of \a f(args)
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        typedef typename std::result_of<F(Args...)>::type return_type;

        if (m_stop)
            DSN_DEFAULT_EXCEPTION_SIMPLE("Cannot enqueue tasks on stopped ThreadPool!");

        auto task = std::make_shared<std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.push([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    void stop();

    size_t num_workers() const;

    bool idle() const;
    size_t idle_count() const;

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

    /// \brief Number of currently idle worker threads
    std::atomic<size_t> m_idle_workers{ 0 };
};
}

#endif
