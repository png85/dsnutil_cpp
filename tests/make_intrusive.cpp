#define BOOST_TEST_MODULE "dsn::make_intrusive"

#include <dsnutil/make_intrusive.hpp>

#include <boost/test/unit_test.hpp>

class TestReferenceCounted : public dsn::reference_counted {
public:
    TestReferenceCounted()
        : m_name("default")
    {
    }

    TestReferenceCounted(std::string name)
        : m_name(std::move(name))
    {
    }

    std::string name() const { return m_name; }

private:
    std::string m_name;
};

BOOST_AUTO_TEST_CASE(make_intrusive_default)
{
    auto ptr = dsn::make_intrusive<TestReferenceCounted>();
    BOOST_CHECK(ptr.get() != nullptr);
    BOOST_CHECK(ptr->reference_count() == 1);
    BOOST_CHECK(ptr->name() == "default");
}

BOOST_AUTO_TEST_CASE(make_intrusive_with_args)
{
    auto ptr = dsn::make_intrusive<TestReferenceCounted>("named");
    BOOST_CHECK(ptr.get() != nullptr);
    BOOST_CHECK(ptr->reference_count() == 1);
    BOOST_CHECK(ptr->name() == "named");
}
