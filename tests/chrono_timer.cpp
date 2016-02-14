#define BOOST_TEST_MODULE "dsn::chrono::timer"

#include <iostream>
#include <thread>

#include <dsnutil/chrono/timer.hpp>

#include <boost/test/unit_test.hpp>

namespace dsn {
namespace chrono {
    std::ostream& operator<<(std::ostream& stream, const timer& t)
    {
        return stream << "timer(" << static_cast<unsigned long long>(t.elapsed().nanoseconds()) << " ns)";
    }
}
}

using timer = dsn::chrono::timer;

BOOST_AUTO_TEST_CASE(default_ctor)
{
    timer t;
    std::cout << "default constructed timer: " << t << std::endl;
}

BOOST_AUTO_TEST_CASE(elapsed)
{
    timer t;
    std::cout << "testing elapsed() with t = " << t << std::endl;
    auto e1 = t.elapsed();
    std::cout << "e1 = " << e1.milliseconds() << "ms" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    auto e2 = t.elapsed();
    std::cout << "e2 = " << e2.milliseconds() << "ms" << std::endl;
    std::cout << "t  = " << t << std::endl;

    auto diff = e2 - e1;
    BOOST_CHECK(std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= 50);
}

BOOST_AUTO_TEST_CASE(reset)
{
    timer t;
    std::cout << "testing reset() with t = " << t << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    auto e1 = t.elapsed();
    std::cout << "e1 = " << e1.milliseconds() << "ms" << std::endl;

    t.reset();
    std::cout << "t after reset = " << t << std::endl;
    auto e2 = t.elapsed();
    std::cout << "e2 = " << e2.milliseconds() << "ms" << std::endl;

    BOOST_CHECK(e2 < e1);
}
