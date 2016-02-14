#pragma once

#include <chrono>
#include <ratio>

namespace dsn {
namespace chrono {

    using nanoseconds = std::chrono::duration<double, std::ratio<1, 1000000000> >;
    using microseconds = std::chrono::duration<double, std::ratio<1, 1000000> >;
    using milliseconds = std::chrono::duration<double, std::ratio<1, 1000> >;
    using seconds = std::chrono::duration<double, std::ratio<1, 1> >;
    using minutes = std::chrono::duration<double, std::ratio<60, 1> >;
    using hours = std::chrono::duration<double, std::ratio<3600, 1> >;
}
}
