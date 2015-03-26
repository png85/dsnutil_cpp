#include <vector>
#include <map>
#include <array>
#include <list>

#include <sstream>

#include <dsnutil/pretty_print.h>

int main() {
    std::vector<int> v0;
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::array<double, 3> a0{ {0.1, 0.2, 0.3} };
    std::map<std::string, int> m;
    m["foo"] = 23;
    m["bar"] = 42;
    std::list<int> l{5, 4, 3, 2, 1};
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

    return 0;
}
