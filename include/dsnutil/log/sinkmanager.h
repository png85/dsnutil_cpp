#ifndef SINKMANAGER_H
#define SINKMANAGER_H

#include <dsnutil/dsnutil_cpp_Export.h>
#include <dsnutil/log/base.h>
#include <dsnutil/singleton.h>

#include <boost/log/sinks.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

namespace dsn {
namespace log {

    class dsnutil_cpp_EXPORT SinkManager : public dsn::Singleton<dsn::log::SinkManager>, public Base<SinkManager> {
        friend class dsn::Singleton<SinkManager>;
        SinkManager();
        ~SinkManager();

    public:
        /// \brief Pointer to a managed log sink
        typedef boost::shared_ptr<boost::log::sinks::sink> sink_ptr;

        bool exists(const std::string& name) const;
        bool add(const std::string& name, const sink_ptr& sink);
        bool remove(const std::string& name);

        std::vector<std::string> sinks() const;
        sink_ptr sink(const std::string& name);

    private:
        /// \brief Storage container type for managed log sinks
        typedef std::map<std::string, sink_ptr> sink_storage;

        /// \brief Storage container for managed log sinks
        sink_storage m_sinks;
    };
}
}

#endif // SINKMANAGER_H
