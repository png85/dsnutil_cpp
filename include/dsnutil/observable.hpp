#pragma once

#include <mutex>
#include <type_traits>
#include <vector>

#include <dsnutil/dsnutil_cpp_Export.h>
#include <dsnutil/observer.hpp>

namespace dsn {

class dsnutil_cpp_EXPORT observable {
protected:
    using observable_mutex_type = std::mutex;
    using observable_scoped_guard = std::lock_guard<observable_mutex_type>;

    std::vector<observer*> m_observers;
    observable_mutex_type m_observersMutex;

public:
    virtual ~observable();
    void addObserver(observer& observer);
    bool removeObserver(observer& observer);
    bool moveObserver(observer& old_observer, observer& new_observer);
};

template <class T> struct is_observable {
    static const bool value = std::is_base_of<observable, T>::value;
};
}
