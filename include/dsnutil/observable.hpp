#pragma once

#include <mutex>
#include <type_traits>
#include <vector>

#include <dsnutil/dsnutil_cpp_Export.h>
#include <dsnutil/observer.hpp>

namespace dsn {

/// \brief Base class for observable objects
///
/// Derive from this to implement the API for an \p observer to observe the lifetime of
/// an object.
class dsnutil_cpp_EXPORT observable {
protected:
    /// \brief Type alias for mutex type used in imeplementation
    using observable_mutex_type = std::mutex;

    /// \brief Type alias for scoped mutex locks in imlpementation
    using observable_scoped_guard = std::lock_guard<observable_mutex_type>;

    /// \brief Observers registered with this object
    ///
    /// This holds (non-owning) pointers to all \p observer instances that want to be
    /// notified when the object is destroyed
    std::vector<observer*> m_observers;

    /// \brief Mutex to synchronize access to observer vector
    ///
    /// This is used internally to synchronize access to \p m_observers from multiple threads.
    observable_mutex_type m_observersMutex;

public:
    virtual ~observable();
    void addObserver(observer& observer);
    bool removeObserver(observer& observer);
    bool moveObserver(observer& old_observer, observer& new_observer);
};

/// \brief Metaprogramming helper for \p observable
///
/// This has its \p value member set to \a true for sub-classes of \p observable
///
/// \see observable
template <class T> struct is_observable {
    /// \brief Indicate whether an object is observable
    ///
    /// This is set to \a true if \a T is derived from \p is_observable or false for any other types.
    static const bool value = std::is_base_of<observable, T>::value;
};
}
