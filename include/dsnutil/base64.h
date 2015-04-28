#ifndef BASE64_H
#define BASE64_H

#include <string>

#include <dsnutil/base64/dsnutil_cpp_base64_Export.h>

namespace dsn {
namespace base64 {
    std::string dsnutil_cpp_base64_EXPORT encode(const unsigned char* s, size_t len);
    std::string dsnutil_cpp_base64_EXPORT encode(const std::string& s);
    std::string dsnutil_cpp_base64_EXPORT decode(const std::string& s);
}
}

#endif // BASE64_H
