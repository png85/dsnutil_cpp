#ifndef BASE64_H
#define BASE64_H

#include <string>

namespace dsn {
namespace base64 {
    std::string encode(const unsigned char* s, size_t len);
    std::string encode(const std::string& s);
    std::string decode(const std::string& s);
}
}

#endif // BASE64_H
