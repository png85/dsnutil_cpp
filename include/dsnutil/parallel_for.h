#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include <thread>

void parallel_for(const size_t size,
                  std::function<void(const size_t)> func,
                  unsigned numThreads=std::thread::hardware_concurrency());

#endif // PARALLEL_FOR_H
