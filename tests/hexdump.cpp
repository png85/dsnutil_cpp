#define BOOST_TEST_MODULE "dsn::hexdump"

#include <array>
#include <iostream>
#include <string>

#include <dsnutil/countof.h>
#include <dsnutil/hexdump.h>

#include <boost/test/unit_test.hpp>

namespace {

const std::string g_message{ "dsn::hexdump test\n"
                             "(c) 2012-2016 #das-system networks\n" };
}

BOOST_AUTO_TEST_CASE(hexdump_string) { hexdump(g_message.c_str(), g_message.length(), std::cout); }

BOOST_AUTO_TEST_CASE(hexdump_int_array)
{
    const size_t array_size{ 3 };
    const auto array_size_bytes = array_size * sizeof(int);
    std::array<int, array_size> a{ 1, 2, 3 };
    hexdump(a.data(), array_size_bytes, std::cout);
}
