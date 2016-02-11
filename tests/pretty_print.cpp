#define BOOST_TEST_MODULE "dsn::pretty_print"

#include <dsnutil/pretty_print.h>

#include <array>
#include <iostream>
#include <map>
#include <vector>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(empty_vector)
{
    std::vector<int> v;
    std::cout << "empty vector<int>: " << v << std::endl;
}

BOOST_AUTO_TEST_CASE(vector_int)
{
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    std::cout << "vector<int>: " << v << std::endl;
}

BOOST_AUTO_TEST_CASE(vector_double)
{
    std::vector<double> v{ 0.1, 0.2, 0.3, 0.4, 0.5 };
    std::cout << "vector<double>: " << v << std::endl;
}

BOOST_AUTO_TEST_CASE(vector_string)
{
    std::vector<std::string> v{ "foo", "bar", "bla" };
    std::cout << "vector<string>: " << v << std::endl;
}

BOOST_AUTO_TEST_CASE(array_int)
{
    std::array<int, 3> a{ 1, 2, 3 };
    std::cout << "array<int>: " << a << std::endl;
}

BOOST_AUTO_TEST_CASE(array_double)
{
    std::array<double, 3> a{ 0.1, 0.2, 0.3 };
    std::cout << "array<double>: " << a << std::endl;
}

BOOST_AUTO_TEST_CASE(array_string)
{
    std::array<std::string, 3> a{ "foo", "bar", "bla" };
    std::cout << "array<string>: " << a << std::endl;
}

BOOST_AUTO_TEST_CASE(list)
{
    std::list<int> l{ 1, 2, 3, 23, 42 };
    std::cout << "list<int>: " << l << std::endl;
}

BOOST_AUTO_TEST_CASE(map)
{
    std::map<std::string, int> m{ { "foo", 23 }, { "bar", 42 } };
    std::cout << "map<string, int>: " << m << std::endl;
}

class Test {
private:
    std::string m_name;

public:
    Test(std::string name)
        : m_name(std::move(name))
    {
    }
    std::string name() const { return m_name; };
};

std::ostream& operator<<(std::ostream& stream, const Test& t)
{
    return stream << "<Test (" << (&t) << "): " << t.name() << ">";
}

BOOST_AUTO_TEST_CASE(vector_with_custom_type)
{
    std::vector<Test> v{ Test("foo"), Test("bar"), Test("bla") };
    std::cout << "vector<Test>: " << v << std::endl;
}

BOOST_AUTO_TEST_CASE(list_with_custom_type)
{
    std::list<Test> l{ Test("foo"), Test("bar"), Test("bla") };
    std::cout << "list<Test>: " << l << std::endl;
}

BOOST_AUTO_TEST_CASE(map_with_custom_type)
{
    std::map<std::string, Test> m{ { "foo", Test("foo") }, { "bar", Test("bar") } };
    std::cout << "map<string, Test>: " << m << std::endl;
}
