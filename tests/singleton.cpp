#define BOOST_TEST_MODULE "dsn::Singleton"

#include <dsnutil/singleton.h>

#include <boost/test/unit_test.hpp>
#include <iostream>

class Test : public dsn::Singleton<Test> {
public:
    void hello() const { std::cout << "Test::hello() @" << this << std::endl; }

    bool testResult() const { return true; }

    bool operator==(const Test& other) const { return (this == &other); }
};

BOOST_AUTO_TEST_CASE(singleton_instanceRef)
{
    std::cout << "-- Testing singleton access by reference" << std::endl;
    bool result{ false };

    auto& test = Test::instanceRef();
    test.hello();
    result = test.testResult();

    BOOST_CHECK(result == true);
}

BOOST_AUTO_TEST_CASE(singleton_instancePtr)
{
    std::cout << "-- Testing singleton access by pointer" << std::endl;
    bool result{ false };

    auto test = Test::instancePtr();
    BOOST_CHECK(test != nullptr);

    test->hello();
    result = test->testResult();

    BOOST_CHECK(result == true);
}

BOOST_AUTO_TEST_CASE(singleton_instance_comparison)
{
    std::cout << "-- Comparing singleton instance pointer and reference";
    auto& ref = Test::instanceRef();
    auto ptr = Test::instancePtr();

    BOOST_CHECK(&ref == ptr);
    BOOST_CHECK(*ptr == ref);
}
