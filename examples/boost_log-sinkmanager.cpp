#include <dsnutil/log/init.h>
#include <dsnutil/log/sinkmanager.h>
#include <dsnutil/pretty_print.h>
#include <dsnutil/null_deleter.h>

int main()
{
    dsn::log::init();

    dsn::log::SinkManager& manager = dsn::log::SinkManager::instanceRef();
    BOOST_LOG_TRIVIAL(info) << "Sinks right after initialization: " << manager.sinks();

    boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
    backend->add_stream(boost::shared_ptr<std::ostream>(&std::cout, dsn::null_deleter()));
    backend->auto_flush(true);

    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
    boost::shared_ptr<sink_t> sink(new sink_t(backend));
    manager.add("std::cout", sink);
    BOOST_LOG_TRIVIAL(info) << "Sinks after creating std::cout sink: " << manager.sinks();

    if (!manager.exists("std::cout")) {
        BOOST_LOG_TRIVIAL(error) << "Missing std::cout sink in manager.sinks(); test failed!";
        return EXIT_FAILURE;
    }

    if (!manager.remove("std::cout")) {
        BOOST_LOG_TRIVIAL(error) << "Failed to remove std::cout sink; test failed!";
        return EXIT_FAILURE;
    }

    BOOST_LOG_TRIVIAL(info) << "Sinks after removing std::cout sink: " << manager.sinks();

    return EXIT_SUCCESS;
}
