#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/trivial.hpp>

#include <dsnutil/log/init.h>

/** \brief Initialize default logging environment
 *
 * Sets up some basic default parameters for the boost::log based logging environment and registers
 * a formatter for boost::log::trivial::severity_level values.
 */
void dsn::log::init(const std::string& filename) {
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_common_attributes();

    static const std::string format{"%TimeStamp% %Severity% (P:%ProcessID%,T:%ThreadID%) (%Class%@%This%): %Message%"};

    boost::log::add_console_log(std::clog, boost::log::keywords::format = format);
    BOOST_LOG_TRIVIAL(trace) << "Initialized default console log settings";

    if(filename.length() > 0) {
        // setup default logfile with given name/pattern, rotation at 1GB size or daily at 00:00:00
        boost::log::add_file_log(boost::log::keywords::file_name = filename,
                                 boost::log::keywords::format = format,
                                 boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
                                 boost::log::keywords::rotation_size = 1024 * 1024 * 1024);
        BOOST_LOG_TRIVIAL(trace) << "Initialized default settings for file: " << filename;
    }
}
