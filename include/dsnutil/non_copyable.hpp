#pragma once

#include <dsnutil/dsnutil_cpp_Export.h>
#include <type_traits>

namespace dsn {

class dsnutil_cpp_EXPORT non_copyable {
private:
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;

public:
    non_copyable() = default;
    non_copyable(non_copyable&&) = default;
    non_copyable& operator=(non_copyable&&) = default;
};

template <typename T> struct is_non_copyable {
    static const bool value = std::is_base_of<non_copyable, T>::value;
};
}
