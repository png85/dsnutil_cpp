#include <dsnutil/observable.hpp>
#include <algorithm>

/// \brief Destroy observable, notify observers
///
/// This calls the \a observable_destroyed() callback on all \p observer instances registered
/// with this object.
dsn::observable::~observable()
{
    observable_scoped_guard guard(m_observersMutex);
    for (auto& observer : m_observers) {
        observer->observable_destroyed(*this);
    }
}

/// \brief Register observer for this object
///
/// This adds \a observer to the registered observers for this object.
///
/// \param observer Reference to the \p observer that shall be registered with this object
void dsn::observable::addObserver(dsn::observer& observer)
{
    observable_scoped_guard guard(m_observersMutex);
    m_observers.push_back(&observer);
}

/// \brief Remove observer from this object
///
/// This removes \a observer frrom the list of registered observers for this object.
///
/// \param observer Referecne to an \p observer registered with this object
///
/// \return true if \p observer was removed or false if it wasn't registered with this object
bool dsn::observable::removeObserver(dsn::observer& observer)
{
    observable_scoped_guard guard(m_observersMutex);

    auto it = std::remove(m_observers.begin(), m_observers.end(), &observer);
    if (it == m_observers.cend()) {
        return false;
    }

    m_observers.erase(it, m_observers.end());

    return true;
}

/// \brief Replace observer
///
/// Replaces \a old_observer with \a new_observer in the list of \p observer instances registered
/// with this object.
///
/// \param old_observer \p observer that shall be removed from this object
/// \param new_observer \p observer that shall be replace \a old_observer
///
/// \return true if the observers were swapped or false if \a old_observer wasn't found
bool dsn::observable::moveObserver(dsn::observer& old_observer, dsn::observer& new_observer)
{
    observable_scoped_guard guard(m_observersMutex);

    auto it = std::find(m_observers.begin(), m_observers.end(), &old_observer);
    if (it == m_observers.cend()) {
        return false;
    }

    *it = &new_observer;

    return true;
}
