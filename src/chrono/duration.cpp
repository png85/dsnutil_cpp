#include <dsnutil/chrono/duration.hpp>
#include <dsnutil/chrono/time_point.hpp>

dsn::chrono::duration::duration(const duration& other)
    : clock_type::duration(other)
{
}

double dsn::chrono::duration::nanoseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1000000000> > >(*this).count();
}

double dsn::chrono::duration::microseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1000000> > >(*this).count();
}

double dsn::chrono::duration::milliseconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1000> > >(*this).count();
}

double dsn::chrono::duration::seconds() const
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1> > >(*this).count();
}

double dsn::chrono::duration::minutes() const
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<60, 1> > >(*this).count();
}

double dsn::chrono::duration::hours() const
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<3600, 1> > >(*this).count();
}

dsn::chrono::time_point dsn::chrono::duration::then() const { return time_point(*this); }

dsn::chrono::duration& dsn::chrono::duration::operator=(const dsn::chrono::duration& other)
{
    clock_type::duration::operator=(other);
    return *this;
}
