#ifndef DSNUTIL_LOG_UTIL_H
#define DSNUTIL_LOG_UTIL_H

#include <dsnutil/log/dsnutil_cpp_log_Export.h>
#include <dsnutil/log/types.h>

namespace dsn {
namespace log {
    namespace util {

        severity dsnutil_cpp_log_EXPORT severityFromString(const std::string& str,
                                                           severity default_severity = severity::debug);
    }
}
}
#endif // DSNUTIL_LOG_UTIL_H
