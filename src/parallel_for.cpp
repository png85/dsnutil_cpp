#include <vector>
#include <dsnutil/parallel_for.h>

/** @brief Parallelized loop
 *
 * This helper can be used to parallelize the execution of a worker function over any data
 * container.
 *
 * @param size Total number of worker tasks
 * @param func Worker function (can be lambda)
 * @param numThreads Max # of threads to use; clamped to # of CPU cores
 */
void dsn::parallel_for(const size_t size, std::function<void (const size_t)> func, unsigned numThreads)
{
    // clamp numThreads to # of CPU cores
    numThreads = std::min(numThreads, std::thread::hardware_concurrency());
    std::vector<std::thread> threads;

    // start worker threads
    for (size_t threadID=0; threadID < numThreads; ++threadID) {
        auto thread_func = [&]() {
            for (size_t i = threadID; i<size; i += numThreads)
                func(i);
        };
        threads.push_back(std::thread(thread_func));
    }

    // wait for all threads to finish
    for (auto& thread : threads)
        thread.join();
}
