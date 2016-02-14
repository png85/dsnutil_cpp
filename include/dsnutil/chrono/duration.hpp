#pragma once

#include <dsnutil/chrono/clock_type.hpp>
#include <dsnutil/chrono/dsnutil_cpp_chrono_Export.h>

namespace dsn {
namespace chrono {

    class dsnutil_cpp_chrono_EXPORT time_point;

    class dsnutil_cpp_chrono_EXPORT duration : public clock_type::duration {
    public:
        duration() = default;
        duration(const duration& other);
        template <typename T, class U>
        duration(const std::chrono::duration<T, U>& other)
            : clock_type::duration(other)
        {
        }

        duration& operator=(const duration& other);
        template <typename T, class U> duration& operator=(const std::chrono::duration<T, U>& other)
        {
            clock_type::duration::operator=(other);
            return *this;
        }

        double nanoseconds() const;
        double microseconds() const;
        double milliseconds() const;
        double seconds() const;
        double minutes() const;
        double hours() const;

        time_point then() const;
    };
}
}
