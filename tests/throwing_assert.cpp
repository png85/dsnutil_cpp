#define BOOST_TEST_MODULE "dsn::throwing_assert"

#ifdef NDEBUG
// we need to undef this in debug builds or assert will get optimized out --png
#undef NDEBUG
#endif

#include <iostream>

#include <boost/test/unit_test.hpp>

// this needs to be included last because boost::test pulls in assert which we redefine --png
#include <dsnutil/throwing_assert.h>

BOOST_AUTO_TEST_CASE(assertion_failure)
{
    std::cout << "-- Testing assertion failure" << std::endl;
    bool exception_triggered{ false };
    try {
        assert(1 == 0); // this should throw
    }

    catch (dsn::Exception& ex) {
        std::cout << "Caught expected exception: " << ex.what() << std::endl;
        exception_triggered = true;
    }
    BOOST_CHECK(exception_triggered == true);
}

BOOST_AUTO_TEST_CASE(assertion_success)
{
    bool exception_triggered{ true };
    std::cout << "-- Testing assertion success" << std::endl;
    try {
        assert(1 == 1); // this should not throw
        exception_triggered = false;
    }

    catch (dsn::Exception& ex) {
        std::cout << "Caught unexpected exception: " << ex.what() << std::endl;
        exception_triggered = true;
    }
    BOOST_CHECK(exception_triggered == false);
}
