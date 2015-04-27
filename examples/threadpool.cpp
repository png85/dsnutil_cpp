#include <cstdlib>
#include <iostream>
#include <chrono>
#include <dsnutil/threadpool.h>

int main()
{
    dsn::ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<std::future<int> > results;

    for (auto i = 0; i < 32; ++i) {
        results.push_back(pool.enqueue([i] {
            std::cout << "Hello " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << " World! " << i << std::endl;
            return i*i;
        }));
    }

    for (auto i = 0; i < results.size(); ++i)
        std::cout << "result #" << i << " = " << results[i].get() << std::endl;

    return EXIT_SUCCESS;
}
