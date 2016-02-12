#define BOOST_TEST_MODULE "dsn::observing_ptr"

#include <dsnutil/observing_ptr.hpp>

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <memory>

class TestObservable : public dsn::observable {
public:
    TestObservable() { std::cout << "TestObservable@" << this << " created." << std::endl; }
    ~TestObservable() { std::cout << "TestObservable@" << this << " destroyed." << std::endl; }

    void hello() const { std::cout << "Hello from TestObservable::hello()@" << this << std::endl; }

    void addObserver(dsn::observer& observer)
    {
        std::cout << "TestObservable@" << this << ": Adding observer: " << (&observer) << std::endl;
        dsn::observable::addObserver(observer);
    }

    void removeObserver(dsn::observer& observer)
    {
        std::cout << "TestObservable@" << this << ": Removing observer: " << (&observer) << std::endl;
        dsn::observable::removeObserver(observer);
    }

    void moveObserver(dsn::observer& old_observer, dsn::observer& new_observer)
    {
        std::cout << "TestObservable@" << this << ": Moving observer: " << (&old_observer) << " -> " << (&new_observer)
                  << std::endl;
        dsn::observable::moveObserver(old_observer, new_observer);
    }
};

// default construction: result should be invalid (nullptr) pointer
BOOST_AUTO_TEST_CASE(construction_default)
{
    std::cout << "-- Testing observing_ptr default construction" << std::endl;
    dsn::observing_ptr<TestObservable> ptr;
    BOOST_CHECK(ptr.get() == nullptr);
}

// construction from pointer: result should be given pointer
BOOST_AUTO_TEST_CASE(construction_from_ptr)
{
    std::cout << "-- Testing observing_ptr construction from pointer" << std::endl;
    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    dsn::observing_ptr<TestObservable> ptr(uptr.get());
    BOOST_CHECK(ptr.get() == uptr.get());
}

// construction from reference: result should be pointer to referenced object
BOOST_AUTO_TEST_CASE(construction_from_ref)
{
    std::cout << "-- Testing observing_ptr construction from reference" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr(obj);
    BOOST_CHECK(ptr.get() == &obj);
}

// copy construction: result should be two equal pointers
BOOST_AUTO_TEST_CASE(construction_copy)
{
    std::cout << "-- Testing observing_ptr copy construction" << std::endl;
    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    dsn::observing_ptr<TestObservable> ptr1(uptr.get());
    BOOST_CHECK(ptr1.get() == uptr.get());

    dsn::observing_ptr<TestObservable> ptr2(ptr1);
    BOOST_CHECK((ptr2.get() == uptr.get() and (ptr1.get() == ptr2.get())));
}

// copy assignment: result should be two equal pointers
BOOST_AUTO_TEST_CASE(assignment_copy)
{
    std::cout << "-- Testing observing_ptr copy assignment" << std::endl;
    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    dsn::observing_ptr<TestObservable> ptr1(uptr.get());
    BOOST_CHECK(ptr1.get() == uptr.get());

    dsn::observing_ptr<TestObservable> ptr2;
    ptr2 = ptr1;
    BOOST_CHECK((ptr2.get() == uptr.get()) and (ptr1.get() == ptr2.get()));
}

// move construction: result should be pointer to given object and invalidated source pointer
BOOST_AUTO_TEST_CASE(construction_move)
{
    std::cout << "-- Testing observing_ptr move construction" << std::endl;
    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    dsn::observing_ptr<TestObservable> ptr1(uptr.get());
    BOOST_CHECK(ptr1.get() == uptr.get());

    dsn::observing_ptr<TestObservable> ptr2(std::move(ptr1));
    BOOST_CHECK((ptr2.get() == uptr.get()) and (ptr1.get() == nullptr));
}

// move assignment: result should be pointer to given object and invalidated source pointer
BOOST_AUTO_TEST_CASE(assignment_move)
{
    std::cout << "-- Testing observing_ptr move assignment" << std::endl;
    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    dsn::observing_ptr<TestObservable> ptr1(uptr.get());
    BOOST_CHECK(ptr1.get() == uptr.get());

    dsn::observing_ptr<TestObservable> ptr2;
    ptr2 = std::move(ptr1);
    BOOST_CHECK((ptr2.get() == uptr.get()) and (ptr1.get() == nullptr));
}

// observer pattern for object destruction
BOOST_AUTO_TEST_CASE(observable_destruction)
{
    dsn::observing_ptr<TestObservable> ptr;
    BOOST_CHECK(ptr == nullptr);

    {
        std::unique_ptr<TestObservable> u{ new TestObservable() };
        ptr.reset(u.get());
        BOOST_CHECK(ptr.get() == u.get());
    }

    // ptr should be nullptr after u has gone out of scope
    BOOST_CHECK(ptr == nullptr);
}

// reset(T*)
BOOST_AUTO_TEST_CASE(reset_ptr)
{
    std::cout << "-- Testing observing_ptr<T>::reset(T*)" << std::endl;
    dsn::observing_ptr<TestObservable> ptr;
    BOOST_CHECK(ptr.get() == nullptr);
    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    ptr.reset(uptr.get());
    BOOST_CHECK(ptr.get() == uptr.get());

    std::unique_ptr<TestObservable> uptr2{ new TestObservable() };
    ptr.reset(uptr2.get());
    BOOST_CHECK(ptr.get() == uptr2.get());
}

// reset(T&)
BOOST_AUTO_TEST_CASE(reset_ref)
{
    std::cout << "-- Testing observing_ptr<T>::reset(T&)" << std::endl;
    TestObservable obj1;
    dsn::observing_ptr<TestObservable> ptr(obj1);
    BOOST_CHECK(ptr.get() == &obj1);

    TestObservable obj2;
    ptr.reset(obj2);
    BOOST_CHECK(ptr.get() == &obj2);
}

// T* get()
BOOST_AUTO_TEST_CASE(get)
{
    std::cout << "-- Testing observing_ptr<T>::get()" << std::endl;
    dsn::observing_ptr<TestObservable> ptr;
    BOOST_CHECK(ptr.get() == nullptr);

    std::unique_ptr<TestObservable> uptr{ new TestObservable() };
    ptr.reset(uptr.get());
    BOOST_CHECK(ptr.get() == uptr.get());
}

// T& = *ptr
BOOST_AUTO_TEST_CASE(dereference)
{
    std::cout << "-- Testing observing_ptr dereference" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr(obj);
    BOOST_CHECK(ptr.get() == &obj);

    TestObservable& ref_from_obj = obj;
    TestObservable& ref_from_ptr = *ptr;
    BOOST_CHECK(&ref_from_obj == &ref_from_ptr);
}

// const T& = *ptr;
BOOST_AUTO_TEST_CASE(const_dereference)
{
    std::cout << "-- Testing observing_ptr const dereference" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr(obj);
    BOOST_CHECK(ptr.get() == &obj);

    const TestObservable& ref_from_obj = obj;
    const TestObservable& ref_from_ptr = *ptr;
    BOOST_CHECK(&ref_from_obj == &ref_from_ptr);
}

// conversion to T* for ->
BOOST_AUTO_TEST_CASE(ptr_access)
{
    std::cout << "-- Testing observing_ptr<T> -> T*" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr(obj);

    TestObservable* p = ptr.operator->();
    BOOST_CHECK(p == &obj);
}

// conversion to const T* ->
BOOST_AUTO_TEST_CASE(const_ptr_access)
{
    std::cout << "-- Testing observing_ptr<T> -> const T*" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr(obj);

    const TestObservable* p = ptr.operator->();
    BOOST_CHECK(p == &obj);
}

// call T->something() through ptr->something()
BOOST_AUTO_TEST_CASE(member_call)
{
    std::cout << "-- Testing -> on observing_ptr<T>" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr(obj);

    BOOST_CHECK(ptr.get() == &obj);
    ptr->hello();
}

// cast operator to bool (for if(ptr) {})
BOOST_AUTO_TEST_CASE(cast_to_bool)
{
    std::cout << "-- Testing observing_ptr conversion to bool" << std::endl;
    dsn::observing_ptr<TestObservable> ptr;
    BOOST_CHECK(static_cast<bool>(ptr) == false);

    TestObservable obj;
    ptr.reset(obj);
    BOOST_CHECK(static_cast<bool>(ptr) == true);
}

// equality comparison operator
BOOST_AUTO_TEST_CASE(comparison_equality)
{
    std::cout << "-- Testing observing_ptr comparison operator (==)" << std::endl;
    dsn::observing_ptr<TestObservable> ptr1, ptr2;
    BOOST_CHECK(ptr1 == ptr2);

    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr3(obj), ptr4(&obj);
    BOOST_CHECK(ptr3 == ptr4);
}

// inequality comparison operator
BOOST_AUTO_TEST_CASE(comparison_inequality)
{
    std::cout << "-- Testin observing_ptr comparison operator (!=)" << std::endl;
    TestObservable obj;
    dsn::observing_ptr<TestObservable> ptr1, ptr2(obj), ptr3(&obj);
    BOOST_CHECK(ptr1 != ptr2);
    BOOST_CHECK(ptr1 != ptr3);
    BOOST_CHECK(!(ptr2 != ptr3));
}
