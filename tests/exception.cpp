#define BOOST_TEST_MODULE "dsn::Exception"

#include <dsnutil/exception.h>

#include <iostream>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(default_exception)
{
    bool exception_triggered{ false };
    try {
        DSN_DEFAULT_EXCEPTION("Test exception #1/4", DSN_EXCEPTION_SOURCE);
    }

    catch (dsn::Exception& ex) {
        std::cout << "Caught expected exception: " << ex.what() << std::endl;
        exception_triggered = true;
    }

    BOOST_CHECK(exception_triggered == true);
}

BOOST_AUTO_TEST_CASE(exception)
{
    bool exception_triggered{ false };
    try {
        DSN_EXCEPTION(dsn::Exception, "Test exception #2/4", DSN_EXCEPTION_SOURCE);
    }

    catch (dsn::Exception& ex) {
        std::cout << "Caught expected exception: " << ex.what() << std::endl;
        exception_triggered = true;
    }

    BOOST_CHECK(exception_triggered == true);
}

BOOST_AUTO_TEST_CASE(default_exception_simple)
{
    bool exception_triggered{ false };
    try {
        DSN_DEFAULT_EXCEPTION_SIMPLE("Test exception #3/4");
    }

    catch (dsn::Exception& ex) {
        std::cout << "Caught expected exception: " << ex.what() << std::endl;
        exception_triggered = true;
    }

    BOOST_CHECK(exception_triggered = true);
}

BOOST_AUTO_TEST_CASE(exception_simple)
{
    bool exception_triggered{ false };
    try {
        DSN_EXCEPTION_SIMPLE(dsn::Exception, "Test exception #4/4");
    }

    catch (dsn::Exception& ex) {
        std::cout << "Caught expected exception: " << ex.what() << std::endl;
        exception_triggered = true;
    }

    BOOST_CHECK(exception_triggered == true);
}
