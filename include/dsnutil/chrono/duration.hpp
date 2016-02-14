#pragma once

#include <dsnutil/chrono/clock_type.hpp>

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

namespace dsn {
namespace chrono {

    class time_point;

    class duration : public clock_type::duration {
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
