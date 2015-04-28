#ifndef INIT_H
#define INIT_H

#include <string>

#include <dsnutil/log/dsnutil_cpp_log_Export.h>

namespace dsn {
namespace log {
    void dsnutil_cpp_log_EXPORT init(const std::string& filename = "");
}
}

#endif // INIT_H
