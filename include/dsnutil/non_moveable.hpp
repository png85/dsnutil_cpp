#pragma once

#include <dsnutil/dsnutil_cpp_Export.h>
#include <type_traits>

namespace dsn {

/// \brief Prevent move construction and assignment
///
/// Derive from this to prevent move construction and assignment for an object.
class dsnutil_cpp_EXPORT non_moveable {
private:
    non_moveable(non_moveable&&) = delete;
    non_moveable& operator=(non_moveable&&) = delete;

public:
    non_moveable() = default;
    non_moveable(const non_moveable&) = default;
    non_moveable& operator=(const non_moveable&) = default;
};

/// \brief Metaprogramming helper for \p non_moveable
///
/// This has its \p value member set to \p true for all sub-classes of \p non_moveable
template <typename T> struct is_non_moveable {
    /// \brief Indicate whether object is non_moveable
    ///
    /// This is set to true if \p T is derived from \p non_moveable or false for any other type.
    static const bool value = std::is_base_of<non_moveable, T>::value;
};
}
