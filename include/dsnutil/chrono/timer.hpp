#pragma once

#include <dsnutil/chrono/duration.hpp>
#include <dsnutil/chrono/time_point.hpp>

namespace dsn {
namespace chrono {

    class timer {
    protected:
        time_point m_point;

    public:
        time_point point() const;
        void reset();
        duration elapsed() const;
    };
}
}
