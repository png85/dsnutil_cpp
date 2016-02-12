#pragma once

#include <type_traits>

namespace dsn {

class observable;

/// \brief Base class for objects that can observe the lifetime of other objects
///
/// Derive from this to implement the API required to observe the lifetime of sub-classes of
/// \p observable
///
/// \see observable
/// \see is_observable
class observer {
    friend class observable;

    /// \brief Callback for observable destruction
    ///
    /// This is called by an \p observable instance when its destroyed
    virtual void observable_destroyed(observable&) = 0;
};

/// \brief Metaprogramming helper for \p observer
///
/// This has its \p value member set to \a true for all sub-classes of \p observer
template <typename T> struct is_observer {
    /// \brief Indicate whether an object is an observer
    ///
    /// This is \a true if \p T is derived from \p observer or \a false for any other type.
    static const bool value = std::is_base_of<observable, T>::value;
};
}
