#include <dsnutil/log/init.h>
#include <dsnutil/log/sinkmanager.h>
#include <dsnutil/null_deleter.h>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

/** \brief Initialize default logging environment
 *
 * Sets up some basic default parameters for the boost::log based logging environment and registers
 * a formatter for boost::log::trivial::severity_level values.
 */
void dsn::log::init(const std::string& filename)
{
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_common_attributes();

    static const std::string format{
        "%TimeStamp% %Severity% (P:%ProcessID%,T:%ThreadID%) (%Class%@%This%): %Message%"
    };

    dsn::log::SinkManager& manager = dsn::log::SinkManager::instanceRef();

    // create backend and sink for std::clog
    typedef boost::log::sinks::text_ostream_backend backend_t;
    boost::shared_ptr<backend_t> backend = boost::make_shared<backend_t>();
    backend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, dsn::null_deleter()));
    backend->auto_flush(true);

    typedef boost::log::sinks::synchronous_sink<backend_t> sink_t;
    boost::shared_ptr<sink_t> sink(new sink_t(backend));
    sink->set_formatter(boost::log::parse_formatter(format));

    manager.add("std::clog", sink);
    BOOST_LOG_TRIVIAL(trace) << "Initialized default console log settings (std::clog)";

    if (filename.length() > 0) {
        // setup backend and sink for the given logfile name, default rotation parameters are:
        //
        // - daily at 00:00:00
        // - at 1GB size
        typedef boost::log::sinks::text_file_backend backend_t;
        boost::shared_ptr<backend_t> fileBackend = boost::make_shared<backend_t>(
            boost::log::keywords::file_name = filename, boost::log::keywords::rotation_size = 1024 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
            boost::log::keywords::format = format);
        typedef boost::log::sinks::synchronous_sink<backend_t> sink_t;
        boost::shared_ptr<sink_t> sink(new sink_t(fileBackend));
        manager.add("file://" + filename, sink);
        BOOST_LOG_TRIVIAL(trace) << "Initialized default settings for file: " << filename;
    }
}
