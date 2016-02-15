#define BOOST_TEST_MODULE "dsn::event::broadcast_channel"

#include <dsnutil/event/broadcast_channel.hpp>
#include <dsnutil/event/broadcast_handler.hpp>
#include <iostream>
#include <memory>

#include <boost/test/unit_test.hpp>

namespace {

struct TestEvent {
    int value{ 0 };

    TestEvent(int v)
        : value(v)
    {
    }
};

int g_recvCount{ 0 };

class TestHandler : public dsn::event::broadcast_handler<TestEvent> {
public:
    TestHandler() = default;

    // broadcast_handler interface
    virtual void operator()(const TestEvent& message) override;
};

void TestHandler::operator()(const TestEvent& message)
{
    std::cout << "Hello from TestHandler(const TestEvent&) -> value: " << message.value << std::endl;
    g_recvCount++;
}

int g_sentCount{ 0 };

void broadcast() { dsn::event::broadcast_channel::broadcast(TestEvent(++g_sentCount)); }

void reset_counters() { g_sentCount = g_recvCount = 0; }

std::unique_ptr<TestHandler> g_testHandler{ nullptr };
}

using channel = dsn::event::broadcast_channel;

BOOST_AUTO_TEST_CASE(broadcast_without_handlers_pre)
{
    std::cout << "-- Testing broadcast without active handlers" << std::endl;
    reset_counters();
    BOOST_CHECK(g_sentCount == 0 and g_recvCount == 0);
    broadcast();
    BOOST_CHECK(g_sentCount == 1 and g_recvCount == 0);
}

BOOST_AUTO_TEST_CASE(create_handler)
{
    std::cout << "-- Testing creation of handler object" << std::endl;
    BOOST_CHECK(!g_testHandler);
    BOOST_CHECK(channel::num_handlers<TestEvent>() == 0);

    g_testHandler.reset(new TestHandler());
    BOOST_CHECK(g_testHandler);
    BOOST_CHECK(channel::num_handlers<TestEvent>() == 1);
}

BOOST_AUTO_TEST_CASE(broadcast_with_one_handler)
{
    std::cout << "-- Testing broadcast with one active handler" << std::endl;
    reset_counters();
    BOOST_CHECK(g_sentCount == 0 and g_recvCount == 0);
    broadcast();
    BOOST_CHECK(g_sentCount == 1 and g_recvCount == 1);
}

namespace {
bool g_secondTriggered{ false };

class SecondHandler : public dsn::event::broadcast_handler<TestEvent> {
public:
    SecondHandler() = default;

    virtual void operator()(const TestEvent& message) override
    {
        std::cout << "Hello from SecondHandler::operator()(const TestEvent&)" << std::endl;
        g_secondTriggered = true;
    }
};
}

BOOST_AUTO_TEST_CASE(broadcast_with_multiple_handlers)
{
    std::cout << "-- Testing broadcast with multiple active handlers" << std::endl;
    SecondHandler second;
    BOOST_CHECK(channel::num_handlers<TestEvent>() == 2);

    reset_counters();
    BOOST_CHECK(g_sentCount == 0 and g_recvCount == 0);
    broadcast();
    BOOST_CHECK(g_sentCount == 1 and g_recvCount == 1);

    BOOST_CHECK(g_secondTriggered);
}

BOOST_AUTO_TEST_CASE(destroy_handler)
{
    std::cout << "-- Testing destruction of handler object" << std::endl;
    BOOST_CHECK(g_testHandler and channel::num_handlers<TestEvent>() == 1);

    g_testHandler.reset();
    BOOST_CHECK(!g_testHandler and channel::num_handlers<TestEvent>() == 0);
}

BOOST_AUTO_TEST_CASE(broadcast_without_handlers_post)
{
    std::cout << "-- Testing broadcast without active handlers after handler removal" << std::endl;
    reset_counters();
    BOOST_CHECK(g_sentCount == 0 and g_recvCount == 0);
    broadcast();
    BOOST_CHECK(g_sentCount == 1 and g_recvCount == 0);
}
