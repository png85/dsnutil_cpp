#include <dsnutil/log/util.h>

#include <map>

severity dsn::log::util::severityFromString(const std::string& str, severity default_severity)
{
    static const std::map<std::string, severity> severityMap{
        { "trace", severity::trace },
        { "debug", severity::debug },
        { "info", severity::info },
        { "error", severity::error },
        { "warning", severity::warning },
        { "fatal", severity::fatal },
    };

    auto it = severityMap.find(str);
    if (it == severityMap.end()) {
        BOOST_LOG_TRIVIAL(warning) << "Logging severity '" << str << "' isn't supported; returning default: " << default_severity;
        return default_severity;
    }

    return it->second;
}
