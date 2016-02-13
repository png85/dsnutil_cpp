#define BOOST_TEST_MODULE "dsn::log::SinkManager"

#include <dsnutil/log/init.h>
#include <dsnutil/log/sinkmanager.h>
#include <dsnutil/null_deleter.h>
#include <dsnutil/pretty_print.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(log_init) { dsn::log::init(); }

using dsn::log::SinkManager;

BOOST_AUTO_TEST_CASE(default_sinks)
{
    auto& manager = SinkManager::instanceRef();
    auto sinks = manager.sinks();
    BOOST_LOG_TRIVIAL(info) << "Sinks after initialization: " << sinks;
    BOOST_CHECK_MESSAGE(sinks.size() == 1, "Expected sink count mismatch"); // should have std::clog but no file
}

BOOST_AUTO_TEST_CASE(add_sink)
{
    auto& manager = SinkManager::instanceRef();
    {
        using backend_type = boost::log::sinks::text_ostream_backend;
        using backend_ptr = boost::shared_ptr<backend_type>;

        backend_ptr backend = boost::make_shared<backend_type>();
        backend->add_stream(boost::shared_ptr<std::ostream>(&std::cout, dsn::null_deleter()));
        backend->auto_flush(true);

        using sink_type = boost::log::sinks::synchronous_sink<backend_type>;
        using sink_ptr = boost::shared_ptr<sink_type>;
        sink_ptr sink(new sink_type(backend));

        manager.add("std::cout", sink);
    }

    auto sinks = manager.sinks();
    BOOST_LOG_TRIVIAL(info) << "Sinks after adding std::cout sink: " << sinks;
    BOOST_CHECK_MESSAGE(sinks.size() == 2, "Expected sink count mismatch");

    auto sink_it = std::find(sinks.begin(), sinks.end(), "std::cout");
    BOOST_CHECK_MESSAGE(sink_it != sinks.end(), "Didn't find std::cout sink in manager.sinks()");
}

BOOST_AUTO_TEST_CASE(sink_exists)
{
    auto& manager = SinkManager::instanceRef();
    auto result = manager.exists("std::cout");
    BOOST_CHECK_MESSAGE(result == true, "Didn't find std::cout sink with manager.exists()");
}

BOOST_AUTO_TEST_CASE(remove_sink)
{
    auto& manager = SinkManager::instanceRef();
    auto result = manager.remove("std::cout");
    BOOST_LOG_TRIVIAL(info) << "Sinks after removing std::cout sink: " << manager.sinks();
    BOOST_CHECK_MESSAGE(result == true, "Failed to remove std::cout sink!");
}
