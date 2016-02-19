#define BOOST_TEST_MODULE "dsn::event::broadcast_handler"

#include <dsnutil/event/broadcast_handler.hpp>
#include <iostream>

#include <boost/test/unit_test.hpp>

namespace {

struct DummyEvent {
};

class DummyHandler : public dsn::event::broadcast_handler<DummyEvent> {
public:
    DummyHandler() = default;

    // broadcast_handler interface
    virtual void operator()(const DummyEvent& message) override;
};

void DummyHandler::operator()(const DummyEvent& message)
{
    std::cout << "Hello from DummyHandler::operator()(const DummyEvent&)!" << std::endl;
}
}

BOOST_AUTO_TEST_CASE(handler_operator)
{
    DummyHandler handler;
    handler(DummyEvent());
}
