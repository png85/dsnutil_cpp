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
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t size=std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args> auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        typedef typename std::result_of<F(Args...)>::type return_type;

        if(m_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        auto task = std::make_shared< std::packaged_task<return_type()> >(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.push([task](){ (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    void stop();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()> > tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool m_stop{false};
};

#endif
