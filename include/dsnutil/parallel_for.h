#ifndef PARALLEL_FOR_H
#define PARALLEL_FOR_H

#include <thread>

#include <dsnutil/dsnutil_cpp_Export.h>

namespace dsn {

void dsnutil_cpp_EXPORT parallel_for(const size_t size,
                                     std::function<void(const size_t)> func,
                                     unsigned numThreads = std::thread::hardware_concurrency());
}

#endif // PARALLEL_FOR_H
