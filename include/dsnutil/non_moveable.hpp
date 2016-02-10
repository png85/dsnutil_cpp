#pragma once

#include <dsnutil/dsnutil_cpp_Export.h>
#include <type_traits>

namespace dsn {

class dsnutil_cpp_EXPORT non_moveable {
private:
    non_moveable(non_moveable&&) = delete;
    non_moveable& operator=(non_moveable&&) = delete;

public:
    non_moveable() = default;
    non_moveable(const non_moveable&&) = default;
    non_moveable& operator=(const non_moveable&) = default;
};

template <typename T> struct is_non_moveable {
    static const bool value = std::is_base_of<non_moveable, T>::value;
};
}
