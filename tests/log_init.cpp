#define BOOST_TEST_MODULE "dsn::log::init"

#include <boost/log/trivial.hpp>
#include <dsnutil/log/init.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(default_init)
{
    dsn::log::init();
    BOOST_LOG_TRIVIAL(info) << "Hello from default_init test";
}

BOOST_AUTO_TEST_CASE(init_with_filename)
{
    dsn::log::init("test-log_init.log");
    BOOST_LOG_TRIVIAL(info) << "Hello from init_with_filename test";
}
