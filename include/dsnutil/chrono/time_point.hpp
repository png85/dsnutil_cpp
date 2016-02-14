#pragma once

#include <dsnutil/chrono/clock_type.hpp>

#if (__cplusplus >= 201402L)
using namespace std::literals::chrono_literals;
#endif

namespace dsn {
namespace chrono {

    class duration;

    class time_point : public clock_type::time_point {
    public:
        time_point();
        time_point(const dsn::chrono::duration& d);
        time_point(const clock_type::time_point& other)
            : clock_type::time_point(other)
        {
        }
        template <typename T, typename U>
        time_point(const std::chrono::time_point<T, U>& other)
            : clock_type::time_point(std::chrono::time_point_cast<clock_type::time_point>(other))
        {
        }
        time_point& operator=(const dsn::chrono::time_point& other);
        template <typename T, typename U> time_point& operator=(const std::chrono::time_point<T, U>& other)
        {
            clock_type::time_point::operator=(other);
            return *this;
        }

        dsn::chrono::duration from_now() const;
        bool in_future() const;
        void from_now(const dsn::chrono::duration& d);
    };

    inline time_point now() { return time_point(); }
}
}
