#define BOOST_TEST_MODULE "dsn::intrusive_ptr"

#include <dsnutil/intrusive_ptr.hpp>
#include <dsnutil/observing_ptr.hpp>
#include <iostream>

#include <boost/test/unit_test.hpp>

namespace {
class TestReferenceCounted : public dsn::reference_counted, public dsn::observable {
public:
    TestReferenceCounted() { std::cout << "Hello from TestReferenceCounted ctor @ " << this << std::endl; }

    virtual ~TestReferenceCounted() { std::cout << "Goodbye from TestReferenceCounted dtor @ " << this << std::endl; }

    void hello() const { std::cout << "Hello, world from TestReferenceCounted @ " << this << std::endl; }
};

using ptr_type = dsn::intrusive_ptr<TestReferenceCounted>;
using observer_type = dsn::observing_ptr<TestReferenceCounted>;
}

BOOST_AUTO_TEST_CASE(default_construction)
{
    ptr_type ptr;
    BOOST_CHECK(ptr.get() == nullptr);
}

BOOST_AUTO_TEST_CASE(construct_from_pointer)
{
    ptr_type ptr(new TestReferenceCounted());
    BOOST_CHECK(ptr.get() != nullptr);
    BOOST_CHECK(ptr->reference_count() == 1);
}

BOOST_AUTO_TEST_CASE(construct_from_reference)
{
    TestReferenceCounted obj;
    obj.ref(); // force ref to avoid deleting stack memory
    BOOST_CHECK(obj.reference_count() == 1);
    {
        ptr_type ptr(obj);
        BOOST_CHECK(ptr.get() == &obj);

        // count should be 2 after ptr took a ref
        BOOST_CHECK(obj.reference_count() == 2);
    }
    // count should be back at 1 after ptr is gone
    BOOST_CHECK(obj.reference_count() == 1);
}

BOOST_AUTO_TEST_CASE(copy_construct)
{

    ptr_type ptr1(new TestReferenceCounted());
    BOOST_CHECK(ptr1.get() != nullptr);
    BOOST_CHECK(ptr1->reference_count() == 1);

    ptr_type ptr2(ptr1);
    BOOST_CHECK(ptr1.get() == ptr2.get());
    BOOST_CHECK(ptr2->reference_count() == 2);
}

BOOST_AUTO_TEST_CASE(copy_assign)
{
    ptr_type ptr1(new TestReferenceCounted());
    BOOST_CHECK(ptr1.get() != nullptr);
    BOOST_CHECK(ptr1->reference_count() == 1);

    ptr_type ptr2;
    BOOST_CHECK(ptr2.get() == nullptr);

    ptr2 = ptr1;
    BOOST_CHECK(ptr2.get() == ptr1.get());
    BOOST_CHECK(ptr2->reference_count() == 2);
}

BOOST_AUTO_TEST_CASE(move_construct)
{
    ptr_type ptr1(new TestReferenceCounted());
    auto src = ptr1.get();
    BOOST_CHECK(src != nullptr);
    BOOST_CHECK(ptr1->reference_count() == 1);

    ptr_type ptr2(std::move(ptr1));
    BOOST_CHECK(ptr1.get() == nullptr);
    BOOST_CHECK(ptr2.get() == src);
    BOOST_CHECK(ptr2->reference_count() == 1);
}

BOOST_AUTO_TEST_CASE(move_assign)
{
    ptr_type ptr1(new TestReferenceCounted());
    auto src = ptr1.get();
    BOOST_CHECK(src != nullptr);
    BOOST_CHECK(ptr1->reference_count() == 1);

    ptr_type ptr2;
    BOOST_CHECK(ptr2.get() == nullptr);

    ptr2 = std::move(ptr1);
    BOOST_CHECK(ptr1.get() == nullptr);
    BOOST_CHECK(ptr2.get() == src);
    BOOST_CHECK(ptr2->reference_count() == 1);
}

BOOST_AUTO_TEST_CASE(reset_ptr)
{
    ptr_type ptr1(new TestReferenceCounted());
    BOOST_CHECK(ptr1.get() != nullptr);

    observer_type obs1(ptr1.get());
    BOOST_CHECK(obs1.get() == ptr1.get());

    ptr1.reset(new TestReferenceCounted());
    observer_type obs2(ptr1.get());
    BOOST_CHECK(ptr1.get() != nullptr);
    BOOST_CHECK(obs1 == nullptr);
    BOOST_CHECK(obs2.get() == ptr1.get());
}

BOOST_AUTO_TEST_CASE(reset_ref)
{
    ptr_type ptr1(new TestReferenceCounted());
    BOOST_CHECK(ptr1.get() != nullptr);

    observer_type obs1(ptr1.get());
    BOOST_CHECK(obs1.get() == ptr1.get());

    ptr1.reset(*(new TestReferenceCounted())); // dereference heap object to prevent stack corruption
    observer_type obs2(ptr1.get());
    BOOST_CHECK(ptr1.get() != nullptr);
    BOOST_CHECK(obs1 == nullptr);
    BOOST_CHECK(obs2.get() == ptr1.get());
}

BOOST_AUTO_TEST_CASE(get)
{
    auto src = new TestReferenceCounted();
    BOOST_CHECK(src != nullptr);
    ptr_type ptr(src);
    BOOST_CHECK(ptr.get() == src);
}

BOOST_AUTO_TEST_CASE(dereference)
{
    auto src = new TestReferenceCounted();
    BOOST_CHECK(src != nullptr);

    auto& src_ref = *src;

    ptr_type ptr(src);
    BOOST_CHECK(ptr.get() == src);

    auto& ptr_ref = *ptr;
    BOOST_CHECK(&ptr_ref == &src_ref);
}

BOOST_AUTO_TEST_CASE(const_dereference)
{
    auto src = new TestReferenceCounted();
    BOOST_CHECK(src != nullptr);

    const auto& src_ref = *src;

    ptr_type ptr(src);
    BOOST_CHECK(ptr.get() == src);

    const auto& ptr_ref = *src;
    BOOST_CHECK(&ptr_ref == &src_ref);
}

BOOST_AUTO_TEST_CASE(member_access)
{
    ptr_type ptr(new TestReferenceCounted());
    BOOST_CHECK(ptr.get() != nullptr);

    TestReferenceCounted* raw_ptr{ ptr.operator->() };
    BOOST_CHECK(raw_ptr == ptr.get());

    ptr->hello();
}

BOOST_AUTO_TEST_CASE(const_member_access)
{
    ptr_type ptr(new TestReferenceCounted());
    BOOST_CHECK(ptr.get() != nullptr);

    const TestReferenceCounted* raw_ptr{ ptr.operator->() };
    BOOST_CHECK(raw_ptr == ptr.get());

    raw_ptr->hello();
}

BOOST_AUTO_TEST_CASE(cast_to_bool)
{
    ptr_type ptr1;
    BOOST_CHECK(!ptr1);

    ptr_type ptr2(new TestReferenceCounted());
    BOOST_CHECK(ptr2);
}
