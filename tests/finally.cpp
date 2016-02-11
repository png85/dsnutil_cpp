#define BOOST_TEST_MODULE "dsn::finally"

#include <dsnutil/finally.h>

#include <boost/test/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(finally_basic)
{
    std::cout << "-- Testing basic finally block" << std::endl;
    bool finally_executed{ false };
    std::cout << "finally_executed before block: " << std::boolalpha << finally_executed << std::endl;

    {
        dsn::finally block([&]() {
            finally_executed = true;
            std::cout << "Hello from basic finally block" << std::endl;
        });
    }

    std::cout << "finally_executed after block: " << std::boolalpha << finally_executed << std::endl;
    BOOST_CHECK(finally_executed == true);
}

BOOST_AUTO_TEST_CASE(finally_exception)
{
    std::cout << "-- Testing finally execution for exceptions" << std::endl;
    bool finally_executed{ false }, exception_triggered{ false }, after_exception{ false };
    std::cout << "status before: finally_executed=" << std::boolalpha << finally_executed
              << ", exception_triggered=" << exception_triggered << ", after_exception=" << after_exception
              << std::endl;

    try {
        dsn::finally block([&]() {
            finally_executed = true;
            std::cout << "Hello from exception test finally block" << std::endl;
        });

        throw std::runtime_error("Test Exception");

        after_exception = true; // this should not happen
    }

    catch (std::runtime_error& ex) {
        exception_triggered = true;
        std::cout << "Hello from catch{} in exception test" << std::endl;
    }

    std::cout << "status after: finally_executed=" << std::boolalpha << finally_executed
              << ", exception_triggered=" << exception_triggered << ", after_exception=" << after_exception
              << std::endl;
    BOOST_CHECK(finally_executed == true);
    BOOST_CHECK(exception_triggered == true);
    BOOST_CHECK(after_exception == false);
}
