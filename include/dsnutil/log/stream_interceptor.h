#ifndef STREAM_INTERCEPTOR_H
#define STREAM_INTERCEPTOR_H

#include <iostream>
#include <memory>

#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>

namespace dsn {
namespace log {

    template <typename levelT = boost::log::trivial::severity_level>
    class stream_interceptor : public std::streambuf {
    public:
        /** \brief Intercept stream to existing logger
         *
         * Intercepts the given std::ostream and dumps all of its data into the given severity logger.
         *
         * \param stream Reference to std::ostream that shall be intercepted
         * \param logger Reference to severity logger that shall be used for the intercepted data
         * \param name Custom name for this stream interceptor
         * \param severity Severity for messages from the intercepted stream
         */
        stream_interceptor(std::ostream& stream,
                           boost::log::sources::severity_logger<levelT>& logger,
                           const std::string& name, boost::log::trivial::severity_level severity = boost::log::trivial::trace)
            : m_orgstream(stream)
            , m_name("unnamed std::ostream")
            , m_logger(logger)
            , m_severity(severity)
        {
            init(name);
        }

        /** \brief Intercept stream to local logger
         *
         * Intercepts the given std::ostream and dumps all of its data into a newly created severity logger.
         *
         * \param stream Referene to std::ostream that shall be intercepted
         * \param name Custom name for this stream interceptor
         * \param severity Severity for messages from the intercepted stream
         */
        stream_interceptor(std::ostream& stream, const std::string& name, boost::log::trivial::severity_level severity = boost::log::trivial::trace)
            : m_orgstream(stream)
            , m_loggerPtr(new boost::log::sources::severity_logger<levelT>)
            , m_logger(*m_loggerPtr.get())
            , m_name("unnamed std::ostream")
            , m_severity(severity)
        {
            init(name);
        }

        /** \brief Remove stream interception
         *
         * Sets the original stream's buffer back to its previous value and destroys \a m_newstream.
         */
        ~stream_interceptor()
        {
            m_orgstream.rdbuf(m_orgbuf);
            m_newstream.reset();
            BOOST_LOG_SEV(m_logger, boost::log::trivial::trace) << "Stream interception '" << m_name << "' stopped.";
        }

    private:
        /** \brief Shared stream interception code for ctors
         *
         * This sets up the actual stream interception by creating an std::ostream instance with its buffer pointing
         * to the original stream's buffer and resetting the original stream to point its buffer to this object.
         *
         * \internal This helper function gets called from \a init and shouldn't be invoked from anywhere else!
         */
        void intercept()
        {
            m_orgbuf = m_orgstream.rdbuf(this);
            try {
                m_newstream.reset(new std::ostream(m_orgbuf));
            }

            catch (std::bad_alloc& ex) {
                BOOST_LOG_SEV(m_logger, boost::log::trivial::fatal) << __FILE__ << ":" << __LINE__ << ": Failed to allocate memory for std::ostream: " << ex.what();
                throw(ex);
            }
        }

        /** \brief Shared initialization code for ctors
         *
         * This gets called from all the constructors to setup the stream name and interception.
         *
         * \internal This helper function gets called from the ctors of this class and shouldn't be invoked from anywhere else!
         */
        void init(const std::string& name)
        {
            intercept();

            if (name.length() > 0)
                m_name = name;

            BOOST_LOG_SEV(m_logger, boost::log::trivial::trace) << "Stream interception '" << m_name << "' started with log level " << m_severity << ".";
        }

    protected:
        //
        // overloaded std::ostream interface
        //

        virtual std::streamsize xsputn(const char_type* __s, std::streamsize __n)
        {
            m_newstream->write(__s, __n);
            BOOST_LOG_SEV(m_logger, m_severity) << "[" << m_name << "] " << __s;
            return __n;
        }

        virtual std::streambuf::int_type overflow(std::streambuf::int_type __c)
        {
            if (!std::streambuf::traits_type::eq_int_type(__c, std::streambuf::traits_type::eof())) {
                std::streambuf::char_type const t = std::streambuf::traits_type::to_char_type(__c);
                this->xsputn(&t, 1);
            }

            return !traits_type::eof();
        }

        virtual int sync()
        {
            return 0;
        }

    private:
        /** \brief Original stream buffer
         *
         * Points to the buffer of the intercepted stream
         */
        std::streambuf* m_orgbuf;

        /** \brief Intercepted stream
         *
         * Reference to the intercepted stream to pass through all data
         */
        std::ostream& m_orgstream;

        /** \brief Interception ostream
         */
        std::unique_ptr<std::ostream> m_newstream;

        /** \brief Stream name
         *
         * This stream name gets added as a prefix to all logged messages.
         */
        std::string m_name;

        /** \brief Severity level for log messages
         */
        boost::log::trivial::severity_level m_severity;

        /** \brief Pointer to local severity_logger
         *
         * This is used in the constructor without a reference to a log source to allocate
         * our own local logger.
         */
        std::unique_ptr<boost::log::sources::severity_logger<levelT> > m_loggerPtr;

        /** \brief Reference to output logger
         */
        boost::log::sources::severity_logger<levelT>& m_logger;
    };
}
}

#endif // STREAM_INTERCEPTOR_H
