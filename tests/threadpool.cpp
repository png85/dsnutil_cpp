#define BOOST_TEST_MODULE "dsn::ThreadPool"

#include <chrono>
#include <dsnutil/threadpool.h>
#include <iostream>
#include <mutex>

#include <boost/test/unit_test.hpp>

using dsn::ThreadPool;
using unique_ThreadPool = std::unique_ptr<ThreadPool>;
using Mutex = std::mutex;
using ScopedGuard = std::lock_guard<Mutex>;

BOOST_AUTO_TEST_CASE(default_construction)
{
    unique_ThreadPool pool{ new ThreadPool() };
    BOOST_CHECK(pool.get() != nullptr);
}

BOOST_AUTO_TEST_CASE(construction_with_size)
{
    unique_ThreadPool pool{ new ThreadPool(2) };
    BOOST_CHECK(pool.get() != nullptr);
}

BOOST_AUTO_TEST_CASE(enqueue_void)
{
    ThreadPool pool;

    Mutex mtx;
    const size_t num_workers{ 1000 };
    size_t workers_completed{ 0 };
    {
        ScopedGuard g(mtx);
        std::cout << "Launching " << num_workers << " tasks on thread pool..." << std::endl;
    }

    // enqueue 1000 tasks without caring about their value
    for (auto i = 0; i < num_workers; i++) {
        pool.enqueue([&]() -> void {
            int sleep_ms{ rand() % 20 };
            {
                ScopedGuard g(mtx);
                std::cout << "Hello from thread #" << std::this_thread::get_id() << ", going to sleep for " << sleep_ms
                          << "ms." << std::endl;
            }

            // sleep for a random time ([0..20] msec) to simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

            {
                ScopedGuard g(mtx);
                std::cout << "Goodbye from thread #" << std::this_thread::get_id() << std::endl;
                ++workers_completed;
            }
        });
    }

    {
        ScopedGuard g(mtx);
        std::cout << "Waiting for workers to finish..." << std::endl;
    }

    // we're using this lambda hack to wait for all workers to finish before exiting the test
    std::function<bool(void)> done = [&]() -> bool {
        ScopedGuard g(mtx);
        return (workers_completed == num_workers);
    };
    while (!done()) {
        std::this_thread::yield();
    }

    BOOST_CHECK(workers_completed == num_workers);
}

BOOST_AUTO_TEST_CASE(enqueue_with_future)
{
    using future_type = std::future<int>;
    using future_vector = std::vector<future_type>;
    const size_t num_tasks{ 100 };

    ThreadPool pool;
    future_vector results;
    results.reserve(num_tasks);

    Mutex mtx;
    // launch test tasks that compute i^2 after sleeping for a random time
    for (auto i = 0; i < num_tasks; ++i) {
        results.push_back(pool.enqueue([i, &mtx]() -> int {
            auto sleep_ms = rand() % 20;
            {
                ScopedGuard g(mtx);
                std::cout << "Hello #" << i << ", sleeping for " << sleep_ms << "ms" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
            int result{ i * i };
            {
                ScopedGuard g(mtx);
                std::cout << "Goodbye #" << i << ", my result is " << result << std::endl;
            }

            return result;
        }));
    }
    {
        // results should now contain num_tasks future<int> instances
        ScopedGuard g(mtx);
        BOOST_CHECK(results.size() == num_tasks);
    }

    // collect results from all futures - this will block if a result isn't available yet
    // but the pool will continue working on other threads asynchronously
    auto i = 0;
    for (auto& result : results) {
        auto value = result.get();
        {
            ScopedGuard g(mtx);
            std::cout << "Result #" << i++ << " = " << value << std::endl;
        }
    }
}
