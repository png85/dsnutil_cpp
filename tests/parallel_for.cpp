#define BOOST_TEST_MODULE "dsn::parallel_for"

#include <array>
#include <chrono>
#include <cmath>
#include <iostream>

#include <dsnutil/parallel_for.h>

#include <boost/math/constants/constants.hpp>
#include <boost/test/unit_test.hpp>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;

using std::chrono::duration_cast;
using std::chrono::milliseconds;

BOOST_AUTO_TEST_CASE(parallel_for)
{
    static const size_t size{ static_cast<size_t>(1e4) };
    std::array<double, size> result;
    std::fill(result.begin(), result.end(), 0.0);

    auto f = [](size_t i) -> double {
        return std::sin(2.0 * boost::math::constants::pi<double>() / static_cast<double>(i + 1));
    };

    std::cout << "launching parallel_for loop for " << size << " elements..." << std::endl;
    TimePoint parallel_start = Clock::now();
    dsn::parallel_for(size, [&](size_t index) { result[index] = f(index); });
    TimePoint parallel_end = Clock::now();
    auto parallel_time = duration_cast<milliseconds>(parallel_end - parallel_start).count();
    std::cout << "parallel_for loop execution finished in " << parallel_time << "ms" << std::endl;
}
