#define BOOST_TEST_MODULE "dsn::reverse"

#include <iostream>
#include <vector>

#include <dsnutil/make_reverse.h>
#include <dsnutil/pretty_print.h>

#include <boost/test/unit_test.hpp>

namespace {
const std::vector<int> g_v1{ 1, 2, 3, 4, 5 };
const std::vector<int> g_v2{ 5, 4, 3, 2, 1 };
}

BOOST_AUTO_TEST_CASE(make_reverse_size)
{
    std::cout << "v1: " << g_v1 << std::endl;
    std::cout << "v2: " << g_v2 << std::endl;

    auto reverse = dsn::make_reverse(g_v1);
    std::vector<int> v3{ reverse.begin(), reverse.end() };
    std::cout << "v3: " << v3 << std::endl;
    BOOST_CHECK_MESSAGE(v3.size() == g_v1.size(), "reversed vector didn't match source vector's size!");
}

BOOST_AUTO_TEST_CASE(make_reverse_comparison)
{
    auto reverse = dsn::make_reverse(g_v1);
    std::vector<int> v3{ reverse.begin(), reverse.end() };
    std::cout << "v3: " << v3 << std::endl;

    bool compare_result = std::equal(v3.begin(), v3.end(), g_v2.begin());
    BOOST_CHECK_MESSAGE(compare_result == true, "std::equal failed for reversed vector!");
}
