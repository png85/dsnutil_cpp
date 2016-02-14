#include <algorithm>
#include <dsnutil/dsnutil_cpp_Export.h>
#include <dsnutil/log/sinkmanager.h>
#include <dsnutil/map_sort.h>

using namespace dsn::log;

SinkManager::SinkManager() {}

SinkManager::~SinkManager()
{
    for (auto& sink_pair : m_sinks) {
        auto& ptr = sink_pair.second;
        boost::log::core::get()->remove_sink(ptr);
    }
}

/// \brief Check whether a log sink exists
///
/// \param name Name of the log sink that shall be checked
///
/// \return true if a log sink with the given \a name exists or false if it wasn't found
bool SinkManager::exists(const std::string& name) const
{
    auto it = m_sinks.find(name);
    return it != m_sinks.end();
}

/// \brief Add log sink
///
/// This registers a new log sink with the manager.
///
/// \note If a sink with the given \a name already exists it will be overwritten.
///
/// \param name Name of the newly registered sink
/// \param sink Pointer to the log sink that shall be added to the manager
///
/// \return true if the sink was added or false on errors
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

/// \brief Remove log sink
///
/// Removes a log sink from the manager.
///
/// \param name Name of the log sink that shall be removed
///
/// \return true if the sink was removed or false if no sink with the given \a name was found
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

/// \brief Get registered sink names
///
/// \return vector with the names of all registered sinks
std::vector<std::string> SinkManager::sinks() const { return dsn::map_keys(m_sinks); }

/// \brief Get pointer to registered sink
///
/// \param name Name of the log sink that shall be returned
///
/// \return sink_ptr to the registered sink or nullptr if no sink with the given \a name was found
SinkManager::sink_ptr SinkManager::sink(const std::string& name)
{
    if (!exists(name)) {
        BOOST_LOG_SEV(log, severity::error) << "Tried access non-existant sink '" << name.c_str()
                                            << "'; returning nullptr!";
        return sink_ptr(dsnutil_cpp_NULLPTR);
    }

    return m_sinks[name];
}
