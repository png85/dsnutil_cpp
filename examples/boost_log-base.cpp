#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <dsnutil/log/init.h>
#include <dsnutil/log/base.h>

class TestA : public dsn::log::Base<TestA> {
public:
    TestA()
    {
        BOOST_LOG_SEV(log, severity::debug) << "Hello from CTOR!";
    }

    ~TestA()
    {
        BOOST_LOG_SEV(log, severity::debug) << "Goodbye from DTOR!";
    }
};

class TestB : public dsn::log::Base<TestB> {
public:
    TestB()
    {
        BOOST_LOG_SEV(log, severity::debug) << "Hello from CTOR!";
    }

    ~TestB()
    {
        BOOST_LOG_SEV(log, severity::debug) << "Goodbye from DTOR!";
    }
};

int main()
{
    dsn::log::init("boost_log-base.log.%N");

    BOOST_LOG_TRIVIAL(debug) << "Hello from main!";

    TestA a;
    TestB b;

    BOOST_LOG_TRIVIAL(debug) << "Goodbye from main!";

    return EXIT_SUCCESS;
}
