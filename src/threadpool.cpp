#include <dsnutil/threadpool.h>

using namespace dsn;

/// \brief Initialize thread pool
///
/// \param size Maximum number of tasks to execute in parallel
ThreadPool::ThreadPool(size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        workers.emplace_back([this] {
            m_idle_workers++;
            for (;;) {
                std::unique_lock<std::mutex> lock(this->queue_mutex);
                while (!this->m_stop && this->tasks.empty()) {
                    this->condition.wait(lock);
                }

                if (this->m_stop && this->tasks.empty()) {
                    m_idle_workers--;
                    return;
                }

                std::function<void()> task(this->tasks.front());
                this->tasks.pop();
                lock.unlock();
                m_idle_workers--;
                task();
                m_idle_workers++;
            }
        });
    }
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        m_stop = true;
    }

    condition.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }
}

/// \brief Get number of worker threads in this pool
size_t ThreadPool::num_workers() const { return workers.size(); }

/// \brief Check whether thread is currently idle
bool ThreadPool::idle() const { return (m_idle_workers.load() == workers.size()); }

/// \brief Get number of currently idle workers
size_t ThreadPool::idle_count() const { return m_idle_workers; }
