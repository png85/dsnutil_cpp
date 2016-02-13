#define BOOST_TEST_MODULE "dsn::log::Base"

#include <dsnutil/log/base.h>
#include <dsnutil/log/init.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(log_init) { dsn::log::init(); }

namespace {
class LoggerTest : public dsn::log::Base<LoggerTest> {
public:
    LoggerTest() { BOOST_LOG_SEV(log, severity::info) << "Hello from LoggerTest constructor"; }

    void error() { BOOST_LOG_SEV(log, severity::error) << "This is a demo error message"; }

    ~LoggerTest() { BOOST_LOG_SEV(log, severity::info) << "Goodbye from LoggerTest destructor"; }
};
}

BOOST_AUTO_TEST_CASE(derived_logger)
{
    std::unique_ptr<LoggerTest> ptr(new LoggerTest());
    BOOST_CHECK(ptr.get() != nullptr);

    ptr->error();
}
