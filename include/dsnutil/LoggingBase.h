#ifndef LOGGINGBASE_H
#define LOGGINGBASE_H

#include <typeinfo>
#include <iostream>

#include <log4cpp/Category.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/OstreamAppender.hh>

namespace dsn {

/** @brief Base log4cpp-based logging API
 *
 * This template implements a base logging API using log4cpp which can be added to any class
 * by deriving it from the template instanced for the class itself.
 * The class will then inherit a member named \a log which can be used for log messages from the
 * class as well as a \a copyLogSettings method which will allow easy copying of logging settings
 * and appenders between multiple instances/objects.
 *
 * @param Derived Derived class for which the logging API shall be generated
 *
 * @author Peter Hille (png!das-system) <peter@das-system-networks.de>
 *
 * @version 1.0 Initial implementation
 */
template <class Derived> class LoggingBase {
public:
    /**
     * @brief Initialize base logging API
     *
     * Initializes the \a log member so it outputs messages with the name of the class using this
     * template and configures its priority depending on the build type (DEBUG for debug builds,
     * INFO for all others). By default all log messages are sent to the std::clog standard output
     * stream.
     */
    LoggingBase() : log(log4cpp::Category::getInstance(typeid(Derived).name())) {
        // initialize default log appender to std::clog
        std::string className = typeid(Derived).name();
        try {
            log4cpp::Appender* appender = new log4cpp::OstreamAppender(className, &std::clog);
            try {
                log4cpp::Layout* layout = new log4cpp::BasicLayout();
                appender->setLayout(layout);
            }

            catch (std::bad_alloc& ex) {
                delete appender;
                throw;
            }

            log.addAppender(appender);
        }

        catch (std::bad_alloc& ex) {
            std::cerr << "Failed to allocate memory for basic log4cpp objects for " << className << ": " << ex.what();
            throw;
        }

        // setup minimum logging priority depending on build type
#ifdef NDEBUG
        log.setPriority(log4cpp::Priority::INFO);
#else
        log.setPriority(log4cpp::Priority::DEBUG);
#endif
        log << log4cpp::Priority::DEBUG << "Initialized default logging settings!";
    }


    ~LoggingBase() {
        log << log4cpp::Priority::DEBUG << "Removing all log appenders...";
        log.removeAllAppenders();
    }


    /**
     * @brief Copy logging settings from another log4cpp::Category
     *
     * Copies all log appenders and the logging priority of the supplied logging category.
     *
     * @note Object ownership for the appenders stays with the other category since they're being
     * copied by-reference here.
     *
     * @attention It is the responsibility of the caller to make sure that all log appenders remain
     * valid until after this object has been destroyed or they have been removed by a consecutive
     * call to this method!
     *
     * @param other Reference to the category from which the settings shall be copied
     */
    void copyLogSettings(log4cpp::Category& other) {
        log << log4cpp::Priority::DEBUG << "Copying logging settings from " << other.getName() << "...";

        // copy all logging appenders from the other category, adding them by reference
        // so object ownership stays as it is
        log.removeAllAppenders();
        log4cpp::AppenderSet appenders = other.getAllAppenders();
        for (log4cpp::AppenderSet::iterator i = appenders.begin(); i != appenders.end(); ++i) {
            log4cpp::Appender* appender = (*i);
            log.addAppender(*appender);
        }

        // copy logging priority from the other category
        log.setPriority(other.getPriority());

        log << log4cpp::Priority::DEBUG << "Successfully copied logging settings from " << other.getName();
    }

protected:
    /**
     * @brief logging category for messages from derived classes
     */
    log4cpp::Category& log;
};

}

#endif // LOGGINGBASE_H
