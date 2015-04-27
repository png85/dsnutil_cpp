#include <vector>
#include <map>
#include <array>
#include <list>

#include <sstream>

#ifdef WITH_BOOST_LOG
#include <boost/log/trivial.hpp>
#endif

#include <dsnutil/pretty_print.h>

int main()
{
    std::vector<int> v0;
    std::vector<int> v1{ 1, 2, 3, 4, 5 };
    std::array<double, 3> a0{ { 0.1, 0.2, 0.3 } };
    std::map<std::string, int> m;
    m["foo"] = 23;
    m["bar"] = 42;
    std::list<int> l{ 5, 4, 3, 2, 1 };
    std::list<std::string> l2{ "foo", "bar", "bla", "baz" };

    std::cout << "Testing pretty printing on std::cout:" << std::endl;
    std::cout << "Empty vector: " << v0 << std::endl
              << "Vector      : " << v1 << std::endl
              << "Array       : " << a0 << std::endl
              << "Map         : " << m << std::endl
              << "list<int>   : " << l << std::endl
              << "list<string>: " << l2 << std::endl;

    std::stringstream ss;
    ss << "Empty vector: " << v0 << std::endl
       << "Vector      : " << v1 << std::endl
       << "Array       : " << a0 << std::endl
       << "Map         : " << m << std::endl
       << "list<int>   : " << l << std::endl
       << "list<string>: " << l2 << std::endl;
    std::cout << std::endl << "Testing pretty printing into std::stringstream:" << std::endl << ss.str();

#ifdef WITH_BOOST_LOG
    BOOST_LOG_TRIVIAL(debug) << "Testing pretty printing to boost::log:";
    BOOST_LOG_TRIVIAL(debug) << "Empty vector: " << v0;
    BOOST_LOG_TRIVIAL(debug) << "Vector      : " << v1;
    BOOST_LOG_TRIVIAL(debug) << "Array       : " << a0;
    BOOST_LOG_TRIVIAL(debug) << "Map         : " << m;
    BOOST_LOG_TRIVIAL(debug) << "list<int>   : " << l;
    BOOST_LOG_TRIVIAL(debug) << "list<string>: " << l2;
#endif

    return 0;
}
