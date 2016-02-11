#define BOOST_TEST_MODULE "dsn::observable"

#include <dsnutil/observable.hpp>

#include <iostream>
#include <memory>

#include <boost/test/unit_test.hpp>

class TestObservable : public dsn::observable {
public:
    TestObservable() { std::cout << "TestObservable created @ " << this << std::endl; }
    ~TestObservable() { std::cout << "TestObservable destroyed @ " << this << std::endl; }
};

class TestObserver : public dsn::observer {
public:
    bool m_observable_destroyed{ false };

    TestObserver() { std::cout << "TestObserver created @" << this << std::endl; }
    ~TestObserver() { std::cout << "TestObserver destroyed @ " << this << std::endl; }

    // observer interface
private:
    virtual void observable_destroyed(dsn::observable& observable) override;
};

void TestObserver::observable_destroyed(dsn::observable& observable)
{
    std::cout << "Hello from TestObserver::observable_destroyed()! Destroyed observable is @ " << (&observable)
              << std::endl;
    m_observable_destroyed = true;
}

BOOST_AUTO_TEST_CASE(default_construction)
{
    std::cout << "-- Testing stack-allocated default construction of observable" << std::endl;
    TestObservable observable;

    {
        std::cout << "-- Testing heap-allocated default construction of observable" << std::endl;
        std::unique_ptr<TestObservable> o{ new TestObservable() };
    }
}

BOOST_AUTO_TEST_CASE(destruction)
{
    std::cout << "-- Testing destruction of stack-allocated observable" << std::endl;
    TestObserver observer;
    {
        TestObservable observable;
        observable.addObserver(observer);
    }
    BOOST_CHECK(observer.m_observable_destroyed == true);

    std::cout << "-- Testing destruction of heap-allocated observable" << std::endl;
    observer.m_observable_destroyed = false;
    {
        std::unique_ptr<TestObservable> o{ new TestObservable() };
        o->addObserver(observer);
    }
    BOOST_CHECK(observer.m_observable_destroyed == true);
}
