#include <dsnutil/threadpool.h>

using namespace dsn;

ThreadPool::ThreadPool(size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::unique_lock<std::mutex> lock(this->queue_mutex);
                while (!this->m_stop && this->tasks.empty())
                    this->condition.wait(lock);

                if (this->m_stop && this->tasks.empty())
                    return;

                std::function<void()> task(this->tasks.front());
                this->tasks.pop();
                lock.unlock();
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        m_stop = true;
    }

    condition.notify_all();

    for (auto& worker : workers)
        worker.join();
}
