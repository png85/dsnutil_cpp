#define BOOST_TEST_MODULE "dsn::reference_counted"

#include <dsnutil/observing_ptr.hpp>
#include <dsnutil/reference_counted.hpp>
#include <iostream>

#include <boost/test/unit_test.hpp>

class TestRefCounted : public dsn::reference_counted {
public:
    TestRefCounted() { std::cout << "Hello from TestRefCounted" << std::endl; }

    ~TestRefCounted() { std::cout << "Goodbye from TestRefCounted" << std::endl; }
};

BOOST_AUTO_TEST_CASE(default_construction)
{
    std::unique_ptr<TestRefCounted> ptr{ new TestRefCounted() };
    BOOST_CHECK(ptr != nullptr);
    BOOST_CHECK(ptr->reference_count() == 0);
}

BOOST_AUTO_TEST_CASE(manual_reference)
{
    std::unique_ptr<TestRefCounted> ptr{ new TestRefCounted() };
    BOOST_CHECK(ptr != nullptr);
    BOOST_CHECK(ptr->reference_count() == 0);

    ptr->ref();
    BOOST_CHECK(ptr->reference_count() == 1);

    ptr->unref(true);
    BOOST_CHECK(ptr->reference_count() == 0);
}

class AutoDestroyTest : public dsn::reference_counted {
    bool& m_destroyed;

public:
    AutoDestroyTest(bool& result)
        : m_destroyed(result)
    {
        m_destroyed = false;
        std::cout << "Hello from AutoDestroyTest" << std::endl;
    }

    ~AutoDestroyTest()
    {
        m_destroyed = true;
        std::cout << "Goodbye from AutoDestroyTest" << std::endl;
    }
};

BOOST_AUTO_TEST_CASE(auto_destruction)
{
    bool result{ false };
    AutoDestroyTest* ptr{ new AutoDestroyTest(result) };
    BOOST_CHECK(ptr != nullptr);
    BOOST_CHECK(ptr->reference_count() == 0);

    ptr->ref();
    BOOST_CHECK(ptr->reference_count() == 1);

    ptr->unref();
    BOOST_CHECK(result == true);
}

class TestRefCountedObservable : public dsn::reference_counted, public dsn::observable {
public:
    TestRefCountedObservable() { std::cout << "Hello from TestRefCountedObservable" << std::endl; }

    ~TestRefCountedObservable() { std::cout << "Goodbye from TestRefCountedObservable" << std::endl; }
};

BOOST_AUTO_TEST_CASE(reference_counted_observable)
{
    auto ptr = dsn::observing_ptr<TestRefCountedObservable>(new TestRefCountedObservable());
    BOOST_CHECK(ptr != nullptr);
    BOOST_CHECK(ptr->reference_count() == 0);

    ptr->ref();
    BOOST_CHECK(ptr->reference_count() == 1);

    // this unref will destroy the instance and should notify ptr
    ptr->unref();
    BOOST_CHECK(ptr == nullptr);
}
