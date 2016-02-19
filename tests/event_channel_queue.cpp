#define BOOST_TEST_MODULE "dsn::event::channel_queue"

#include <dsnutil/event/channel_queue.hpp>

#include <boost/test/unit_test.hpp>

namespace {
struct DummyEvent {
};
}

BOOST_AUTO_TEST_CASE(singleton_instance)
{
    auto ptr = dsn::event::channel_queue<DummyEvent>::instancePtr();
    BOOST_CHECK(ptr != nullptr);

    auto& ref = dsn::event::channel_queue<DummyEvent>::instanceRef();
    BOOST_CHECK(&ref == ptr);
}

BOOST_AUTO_TEST_CASE(num_handlers)
{
    auto& q = dsn::event::channel_queue<DummyEvent>::instanceRef();
    BOOST_CHECK(q.num_handlers() == 0);
}
