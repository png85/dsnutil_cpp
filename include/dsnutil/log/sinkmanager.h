#ifndef SINKMANAGER_H
#define SINKMANAGER_H

#include <dsnutil/singleton.h>
#include <dsnutil/log/base.h>

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/log/sinks.hpp>

namespace dsn {
namespace log {

    class SinkManager : public dsn::Singleton<SinkManager>, public Base<SinkManager> {
        friend class dsn::Singleton<SinkManager>;
        SinkManager();
        ~SinkManager();

    public:
        typedef boost::shared_ptr<boost::log::sinks::sink> sink_ptr;

        bool exists(const std::string& name) const;
        bool add(const std::string& name, const sink_ptr& sink);
        bool remove(const std::string& name);

        std::vector<std::string> sinks() const;
        sink_ptr sink(const std::string& name);

    private:
        std::map<std::string, sink_ptr> m_sinks;
    };
}
}

#endif // SINKMANAGER_H
