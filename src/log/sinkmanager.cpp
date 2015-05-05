#include <dsnutil/log/sinkmanager.h>
#include <dsnutil/map_sort.h>

using namespace dsn::log;

SinkManager::SinkManager()
{
}

SinkManager::~SinkManager()
{
    for (auto& sink : m_sinks) {
        BOOST_LOG_SEV(log, severity::trace) << "Removing leftover log sink: " << sink.first;
        boost::log::core::get()->remove_sink(sink.second);
    }
    m_sinks.clear();
}

bool SinkManager::exists(const std::string& name) const
{
    for (auto& sink : m_sinks)
        if (sink.first == name)
            return true;

    return false;
}

bool SinkManager::add(const std::string& name, const SinkManager::sink_ptr& sink)
{
    if (exists(name)) {
        if (!remove(name)) {
            BOOST_LOG_SEV(log, severity::error) << "Failed to remove existing '" << name << "' sink!";
            return false;
        }
    }

    boost::log::core::get()->add_sink(sink);
    m_sinks[name] = sink;

    return true;
}

bool SinkManager::remove(const std::string& name)
{
    if (!exists(name)) {
        BOOST_LOG_SEV(log, severity::warning) << "Tried to remove non-existant sink: " << name;
        return false;
    }

    boost::log::core::get()->remove_sink(m_sinks[name]);
    m_sinks.erase(name);

    return true;
}

std::vector<std::string> SinkManager::sinks() const
{
    return dsn::map_keys(m_sinks);
}
