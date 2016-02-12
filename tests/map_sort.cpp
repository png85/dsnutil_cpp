#define BOOST_TEST_MODULE "dsn::map_sort"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <dsnutil/map_sort.h>
#include <dsnutil/pretty_print.h>

#include <boost/test/unit_test.hpp>

using std::string;
using std::map;
using std::pair;

namespace {

const map<string, float> g_source = {
    pair<string, float>("YHOO", 5.0f), pair<string, float>("MSFT", 7.5f), pair<string, float>("GOOG", 9.9f),
    pair<string, float>("DSN", 13.37f), pair<string, float>("IBM", 6.0f), pair<string, float>("ACME", 1.5f),
};
}

BOOST_AUTO_TEST_CASE(map_keys)
{
    std::cout << "-- Testing map_keys into new vector" << std::endl;
    std::cout << "Source map: " << g_source << std::endl;

    auto unsorted_keys = dsn::map_keys(g_source);
    BOOST_CHECK_MESSAGE(unsorted_keys.size() == g_source.size(), "Key vector size mismatch");

    std::cout << "Unsorted keys: " << unsorted_keys << std::endl;
    for (const auto& pair : g_source) {
        auto it = std::find(unsorted_keys.begin(), unsorted_keys.end(), pair.first);
        BOOST_CHECK_MESSAGE(it != unsorted_keys.end(),
            std::string("Source key ") + pair.first + std::string(" not found in unsorted_keys!"));
    }
}

BOOST_AUTO_TEST_CASE(map_keys_with_ref)
{
    std::cout << "-- Testing map_keys into existing vector" << std::endl;
    std::cout << "Source map: " << g_source << std::endl;

    std::vector<std::string> unsorted_keys;
    unsorted_keys.reserve(g_source.size());
    dsn::map_keys(g_source, unsorted_keys);

    auto source_size = g_source.size();
    auto unsorted_keys_size = unsorted_keys.size();
    BOOST_CHECK_MESSAGE(source_size == unsorted_keys_size, "Key vector size mismatch");

    std::cout << "Unsorted keys: " << unsorted_keys << std::endl;
    for (const auto& pair : g_source) {
        auto it = std::find(unsorted_keys.begin(), unsorted_keys.end(), pair.first);
        BOOST_CHECK_MESSAGE(it != unsorted_keys.end(),
            std::string("Source key ") + pair.first + std::string(" not found in unsorted_keys!"));
    }
}

BOOST_AUTO_TEST_CASE(map_sort)
{
    std::cout << "-- Testing map_sort into new vector" << std::endl;
    std::cout << "Source map: " << g_source << std::endl;

    auto unsorted_keys = dsn::map_keys(g_source);
    auto sorted_keys = dsn::map_sort(g_source);
    std::cout << "Sorted keys: " << sorted_keys << std::endl;
    BOOST_CHECK_MESSAGE(unsorted_keys.size() == g_source.size(), "Unsorted key vector size mismatch");
    BOOST_CHECK_MESSAGE(sorted_keys.size() == unsorted_keys.size(), "Sorted key vector size mismatch");

    static const std::vector<std::string> expected_order{ "ACME", "YHOO", "IBM", "MSFT", "GOOG", "DSN" };
    std::cout << "Expected keys order: " << expected_order << std::endl;
    bool compare_result = std::equal(expected_order.begin(), expected_order.end(), sorted_keys.begin());
    BOOST_CHECK_MESSAGE(compare_result == true, "std::equal failed for sorted key vector");
}

BOOST_AUTO_TEST_CASE(map_sort_with_ref)
{
    std::cout << "-- Testing map_sort into existing vector" << std::endl;
    std::cout << "Source map: " << g_source << std::endl;
    std::vector<std::string> unsorted_keys, sorted_keys;
    unsorted_keys.reserve(g_source.size());
    dsn::map_keys(g_source, unsorted_keys);
    BOOST_CHECK_MESSAGE(unsorted_keys.size() == g_source.size(), "unsorted_keys size mismatch");

    sorted_keys.reserve(g_source.size());
    dsn::map_sort(g_source, sorted_keys);
    BOOST_CHECK_MESSAGE(sorted_keys.size() == unsorted_keys.size(), "sorted_keys size mismatch");
    std::cout << "Sorted keys: " << sorted_keys << std::endl;

    static const std::vector<std::string> expected_order{ "ACME", "YHOO", "IBM", "MSFT", "GOOG", "DSN" };
    std::cout << "Expected order: " << expected_order << std::endl;
    bool compare_result = std::equal(expected_order.begin(), expected_order.end(), sorted_keys.begin());
    BOOST_CHECK_MESSAGE(compare_result == true, "std::equal failed for sorted_keys");
}

namespace {

std::function<bool(std::string, std::string)> desc_sort = [](std::string a, std::string b) -> bool {
    auto it_a = g_source.find(a);
    auto it_b = g_source.find(b);

    return (it_a->second > it_b->second);
};
}

BOOST_AUTO_TEST_CASE(custom_comparator)
{
    std::cout << "-- Testing map_sort into new vector with custom comparator" << std::endl;
    std::cout << "Soure map: " << g_source << std::endl;

    auto unsorted_keys = dsn::map_keys(g_source);
    BOOST_CHECK_MESSAGE(unsorted_keys.size() == g_source.size(), "unsorted_keys size mismatch");

    std::vector<std::string> sorted_keys = dsn::map_sort(g_source, desc_sort);
    std::cout << "Sorted keys: " << sorted_keys << std::endl;
    BOOST_CHECK_MESSAGE(sorted_keys.size() == unsorted_keys.size(), "sorted_keys size mismatch");

    static const std::vector<std::string> expected_order{ "DSN", "GOOG", "MSFT", "IBM", "YHOO", "ACME" };
    std::cout << "Expected order: " << expected_order << std::endl;

    bool compare_result = std::equal(expected_order.begin(), expected_order.end(), sorted_keys.begin());
    BOOST_CHECK_MESSAGE(compare_result == true, "sorted_keys didn't match expected_order");
}

BOOST_AUTO_TEST_CASE(custom_comparator_with_ref)
{
    std::cout << "-- Testing map_sort into existing vector with custom comparator" << std::endl;
    std::cout << "Source map: " << g_source << std::endl;

    std::vector<std::string> unsorted_keys, sorted_keys;
    dsn::map_keys(g_source, unsorted_keys);
    BOOST_CHECK_MESSAGE(unsorted_keys.size() == g_source.size(), "unsorted_keys size mismatch");

    dsn::map_sort(g_source, sorted_keys, desc_sort);
    std::cout << "Sorted keys: " << sorted_keys << std::endl;
    BOOST_CHECK_MESSAGE(sorted_keys.size() == g_source.size(), "sorted_keys size mismatch");

    static const std::vector<std::string> expected_order{ "DSN", "GOOG", "MSFT", "IBM", "YHOO", "ACME" };
    std::cout << "Expected order: " << expected_order << std::endl;

    bool compare_result = std::equal(expected_order.begin(), expected_order.end(), sorted_keys.begin());
    BOOST_CHECK_MESSAGE(compare_result == true, "sorted_keys didn't match expected_order");
}
