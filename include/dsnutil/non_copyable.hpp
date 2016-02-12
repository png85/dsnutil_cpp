#pragma once

#include <dsnutil/dsnutil_cpp_Export.h>
#include <type_traits>

namespace dsn {

/// \brief Prevent copy construction and assignment
///
/// Derive from this to prohibit copy construction and assignment of an object.
class dsnutil_cpp_EXPORT non_copyable {
private:
    non_copyable(const non_copyable&) = delete;
    non_copyable& operator=(const non_copyable&) = delete;

public:
    non_copyable() = default;
    non_copyable(non_copyable&&) = default;
    non_copyable& operator=(non_copyable&&) = default;
};

/// \brief Metaprogramming helper for \p non_copyable
///
/// This has its \p value member set to \a true for all sub-classes of \p non_copyable.
template <typename T> struct is_non_copyable {
    /// \brief Indicate whether an object is non_copyable
    ///
    /// This is set to \a true if \a T is derived from \p non_copyable or false for any other type
    static const bool value = std::is_base_of<non_copyable, T>::value;
};
}
