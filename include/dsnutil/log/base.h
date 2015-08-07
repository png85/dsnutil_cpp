#ifndef BASE_H
#define BASE_H

#include <dsnutil/compiler_features.h>
#include <dsnutil/log/types.h>

#include <typeinfo>
#include <sstream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/constant.hpp>
#include <boost/log/sources/severity_logger.hpp>


namespace dsn {
namespace log {

    /** \brief Base template for classes using boost::log based logging
     *
     * This template implements a basic logging API using boost::log which can be added to any class
     * by deriving from the template instanced for the class itself.
     *
     * The class will then inherit a member named \a log which can be used to log messages using the
     * BOOST_LOG_SEV macro and the severity levels defined in boost::log::trivial::severity_level.
     *
     * Log messages will have a string representation of the mangled class name and the value of the
     * 'this' pointer added as %Class% and %This% so it's possible to tag log messages with precise
     * debug information.
     *
     * \param T Derived class for which the logging API shall be generated
     *
     * \version 1.0 Initial implementation
     *
     * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
     */
    template <typename T>
    class Base {
    public:
        Base<T>()
        {
            static const std::string name{ typeid(T).name() };

            log.add_attribute("Class", boost::log::attributes::constant<std::string>(name));

            std::stringstream ss;
            ss << this;
            log.add_attribute("This", boost::log::attributes::constant<std::string>(ss.str()));
        }

    protected:
        /** \brief Severity logger for all messages from derived class
         *
         * This logger can be used via BOOST_LOG_SEV(log, <severity_level>) << message to log diagnostic
         * information from the instance of the derived class.
         */
        boost::log::sources::severity_logger<severity> log;
    };
}
}

#endif // BASE_H
