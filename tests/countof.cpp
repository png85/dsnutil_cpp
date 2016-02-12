#define BOOST_TEST_MODULE "dsn::countof"

#include <dsnutil/countof.h>

#include <iostream>

#include <boost/test/unit_test.hpp>

// test countof for foo[known_size]
BOOST_AUTO_TEST_CASE(basic_countof)
{
    const size_t size{ 3 };
    int foo[size] = { 1, 2, 3 };
    BOOST_CHECK(dsn::countof(foo) == size);
}

// test countof for foo[]
BOOST_AUTO_TEST_CASE(deduced_countof)
{
    const size_t size{ 3 };
    int foo[] = { 1, 2, 3 };
    BOOST_CHECK(dsn::countof(foo) == size);
}
