#include <cassert>
#include <cstdlib>
#include <dsnutil/observing_ptr.hpp>
#include <iostream>

class TestObservable : public dsn::observable {
public:
    TestObservable() { std::clog << __PRETTY_FUNCTION__ << std::endl; }
    ~TestObservable() { std::clog << __PRETTY_FUNCTION__ << std::endl; }
    void hello() { std::clog << "Hello, TestObservable!" << std::endl; }
};

int main(void)
{
    std::unique_ptr<TestObservable> test{ new TestObservable() };
    std::clog << "test.get() = " << test.get() << std::endl;

    dsn::observing_ptr<TestObservable> test_observer(test.get());
    std::clog << "test_observer.get() = " << test_observer.get() << std::endl;
    assert(test.get() == test_observer.get());
    test_observer->hello();

    test.reset();
    std::clog << "test.get() = " << test.get() << ", test_observer.get() = " << test_observer.get() << std::endl;
    assert(test.get() == nullptr && test_observer.get() == nullptr);

    test.reset(new TestObservable());
    test_observer.reset(test.get());
    std::clog << "test.get() = " << test.get() << ", test_observer.get() = " << test_observer.get() << std::endl;
    assert(test.get() == test_observer.get());

    auto o2 = test_observer;
    std::clog << "o2.get(): " << o2.get() << std::endl;
    assert(o2.get() == test.get());

    auto o3 = std::move(o2);
    std::clog << "o3.get(): " << o3.get() << ", o2.get(): " << o2.get() << std::endl;
    assert(o3.get() == test.get() && o2.get() == nullptr);

    test.reset();
    std::clog << "test.get() = " << test.get() << ", test_observer.get() = " << test_observer.get() << ", "
              << "o3.get(): " << o3.get() << std::endl;
    assert(test.get() == nullptr);
    assert(test_observer.get() == nullptr);
    assert(o3.get() == nullptr);

    return EXIT_SUCCESS;
}
