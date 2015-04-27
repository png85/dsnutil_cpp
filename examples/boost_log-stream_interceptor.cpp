#include <iostream>

#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

#include <dsnutil/log/init.h>
#include <dsnutil/log/stream_interceptor.h>

int main()
{
    dsn::log::init();

    boost::log::sources::severity_logger<boost::log::trivial::severity_level> log;

    // intercept std::cout into log
    dsn::log::stream_interceptor<> cout_interceptor(std::cout, log, "std::cout", boost::log::trivial::info);
    std::cout << "Hello to stdout..." << std::endl;

    // intercept std::cerr using separate logger
    dsn::log::stream_interceptor<> cerr_interceptor(std::cerr, "std::cerr", boost::log::trivial::debug);
    std::cerr << "Hello to cerr (with local logger!)..." << std::endl;

    return EXIT_SUCCESS;
}
