#pragma once

#include <atomic>
#include <dsnutil/dsnutil_cpp_Export.h>

namespace dsn {

/// \brief Base class for reference counted objects
///
/// This adds a simple reference counting mechanism to its sub-classes. Once the reference count
/// its zero the object will delete istself.
class dsnutil_cpp_EXPORT reference_counted {
protected:
    /// \brief Type alias for reference count
    using refcount_t = size_t;

    reference_counted() = default;
    virtual ~reference_counted() = default;

private:
    /// \brief Reference count
    mutable std::atomic<refcount_t> m_reference_count{ 0 };

public:
    void ref() const;
    void unref(bool no_delete = false) const;
    refcount_t reference_count() const;
};

/// \brief Metaprogramming helper to check if T is reference_counted
///
/// This defines its \a value member to \a true for all sub-classes of \p reference_counted or
/// false for anything else.
template <typename T> struct is_reference_counted {
    /// \brief Indicate whether \a T is reference_counted
    ///
    /// This is set to \a true if \p T is derived from \a reference_counted or false for any other types.
    static const bool value = std::is_base_of<reference_counted, T>::value;
};
}
