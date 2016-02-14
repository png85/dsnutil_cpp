#include <dsnutil/chrono/duration.hpp>
#include <dsnutil/chrono/time_point.hpp>

dsn::chrono::time_point::time_point()
    : clock_type::time_point(clock_type::now())
{
}

dsn::chrono::time_point::time_point(const dsn::chrono::duration& d)
    : clock_type::time_point(clock_type::now() + d)
{
}

dsn::chrono::duration dsn::chrono::time_point::from_now() const { return *this - time_point(); }

bool dsn::chrono::time_point::in_future() const
{
    return dsn::chrono::duration(*this - time_point()).nanoseconds() > 0;
}

void dsn::chrono::time_point::from_now(const dsn::chrono::duration& d)
{
    clock_type::time_point::operator=(clock_type::now() + d);
}

dsn::chrono::time_point& dsn::chrono::time_point::operator=(const dsn::chrono::time_point& other)
{
    clock_type::time_point::operator=(other);
    return *this;
}
