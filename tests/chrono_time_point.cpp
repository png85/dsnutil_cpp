#define BOOST_TEST_MODULE "dsn::chrono::time_point"

#include <dsnutil/chrono.hpp>
#include <iomanip>
#include <iostream>

#include <boost/test/unit_test.hpp>

namespace {
dsn::chrono::time_point g_epoch;
}

namespace dsn {
namespace chrono {

    inline std::ostream& operator<<(std::ostream& stream, const dsn::chrono::time_point& point)
    {
        auto num = clock_type::period::num;
        auto den = clock_type::period::den;

        auto d = point - g_epoch; // duration since g_epoch was initialized

        return stream << d.count() << " [" << num << "/" << den << "] units (" << point.time_since_epoch().count()
                      << ")";
    }
}
}

BOOST_AUTO_TEST_CASE(default_construction)
{
    dsn::chrono::time_point t;
    std::cout << "default constructed time point: " << t << std::endl;
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
    dsn::chrono::time_point t(g_epoch);
    std::cout << "copy construction source: " << g_epoch << std::endl << "copy construction target: " << t << std::endl;
    BOOST_CHECK(t == g_epoch);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
    dsn::chrono::time_point t1, t2;
    std::cout << "copy assignment before:" << std::endl << "\tt1= " << t1 << std::endl << "\tt2= " << t2 << std::endl;
    BOOST_CHECK(t1 != t2);
    t2 = t1;
    std::cout << "copy assignment after:" << std::endl << "\tt1= " << t1 << std::endl << "\tt2= " << t2 << std::endl;
    BOOST_CHECK(t2 == t1);
}

BOOST_AUTO_TEST_CASE(from_now)
{
    dsn::chrono::time_point t1, t2(t1);
    t2.from_now(std::chrono::hours(1));
    std::cout << "from_now test" << std::endl
              << "t1 = " << t1 << std::endl
              << "t2 = " << t2 << " (t1 + 1h)" << std::endl;
    BOOST_CHECK(t2 > t1);
    auto d1 = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "t1 and t2 are " << d1 << " seconds apart" << std::endl;
    BOOST_CHECK(d1 == 3600);

    auto d2 = g_epoch.from_now();
    unsigned long long ns_since_epoch = -d2.nanoseconds();
    std::cout << "g_epoch was initialized " << ns_since_epoch << "ns ago" << std::endl;
    BOOST_CHECK(ns_since_epoch > 0);
}

BOOST_AUTO_TEST_CASE(in_future)
{
    dsn::chrono::time_point t;
    std::cout << "g_epoch = " << g_epoch << " in_future: " << std::boolalpha << g_epoch.in_future() << std::endl
              << "      t = " << t << " in_future: " << std::boolalpha << t.in_future() << std::endl;
    BOOST_CHECK(g_epoch.in_future() == false);
    BOOST_CHECK(t.in_future() == false);

    t.from_now(std::chrono::hours(1));
    std::cout << "t in 1 hour = " << t << " in_future: " << std::boolalpha << t.in_future() << std::endl;
    BOOST_CHECK(t.in_future() == true);
}
