#define BOOST_TEST_MODULE "dsn::log::stream_interceptor"

#include <iostream>

#include <dsnutil/log/init.h>
#include <dsnutil/log/sinkmanager.h>
#include <dsnutil/log/stream_interceptor.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(log_init) { dsn::log::init(); }

using dsn::log::SinkManager;

BOOST_AUTO_TEST_CASE(intercept_with_external_logger)
{
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> log;

    // intercept std::cout into log
    {
        dsn::log::stream_interceptor<> cout_interceptor(std::cout, log, "std::cout", boost::log::trivial::info);
        std::cout << "This should show up in stdout and the log" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(intercept_with_local_logger)
{
    // intecept std::cerr using local logger
    dsn::log::stream_interceptor<> cerr_interceptor(std::cerr, "std::cerr", boost::log::trivial::warning);
    std::cerr << "This should show up on stderr and in the log" << std::endl;
}
