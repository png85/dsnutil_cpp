#define BOOST_TEST_MODULE "dsn::chrono::duration"

#include <dsnutil/chrono.hpp>
#include <iostream>

#include <boost/test/unit_test.hpp>

namespace dsn {
namespace chrono {

    std::ostream& operator<<(std::ostream& stream, const duration& d)
    {
        auto num = clock_type::period::num;
        auto den = clock_type::period::den;
        return stream << static_cast<unsigned long long>(d.count()) << " [" << num << "/" << den
                      << "] ^= " << static_cast<unsigned long long>(d.nanoseconds()) << "ns";
    }
}
}

BOOST_AUTO_TEST_CASE(default_construction)
{
    dsn::chrono::duration d;
    std::cout << "default constructed duration: " << d << std::endl;
}

BOOST_AUTO_TEST_CASE(construction_from_std_duration)
{
    dsn::chrono::duration d(std::chrono::seconds(5));
    std::cout << "duration(std::chrono::seconds(5)) = " << d << std::endl;
    BOOST_CHECK(d.seconds() == 5);
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
    dsn::chrono::duration d(std::chrono::seconds(5)), c(d);
    std::cout << "testing copy construction:" << std::endl
              << "d = " << d << std::endl
              << "c = " << c << " (copy of d)" << std::endl;
    BOOST_CHECK(c == d);
}

BOOST_AUTO_TEST_CASE(copy_assign)
{
    dsn::chrono::duration a(std::chrono::seconds(23)), b(std::chrono::seconds(42));
    std::cout << "testing copy assignment: " << std::endl << "a = " << a << std::endl << "b = " << b << std::endl;
    BOOST_CHECK(a != b);
    b = a;
    std::cout << "a = " << a << std::endl << "b = " << b << " (after b=a)" << std::endl;
    BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(aux_methods)
{
    std::cout << "testing duration auxiliary methods" << std::endl;
    dsn::chrono::duration nano(std::chrono::nanoseconds(23)), micro(std::chrono::microseconds(23)),
        milli(std::chrono::milliseconds(23)), seconds(std::chrono::seconds(23)), minutes(std::chrono::minutes(23)),
        hours(std::chrono::hours(23));

    BOOST_CHECK(nano.nanoseconds() == 23);
    BOOST_CHECK(micro.microseconds() == 23);
    BOOST_CHECK(milli.milliseconds() == 23);
    BOOST_CHECK(seconds.seconds() == 23);
    BOOST_CHECK(minutes.minutes() == 23);
    BOOST_CHECK(hours.hours() == 23);
}

BOOST_AUTO_TEST_CASE(then)
{
    std::cout << "testing duration::then()" << std::endl;
    dsn::chrono::duration d(std::chrono::hours(1));
    dsn::chrono::time_point tp_now, tp_then = d.then();

    auto diff = tp_then - tp_now;
    BOOST_CHECK(std::chrono::duration_cast<std::chrono::hours>(diff).count() == 1);
}
