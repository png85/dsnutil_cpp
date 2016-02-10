#include <dsnutil/observable.hpp>

dsn::observable::~observable()
{
    observable_scoped_guard guard(m_observersMutex);
    for (auto& observer : m_observers) {
        observer->observable_destroyed(*this);
    }
}

void dsn::observable::addObserver(dsn::observer& observer)
{
    observable_scoped_guard guard(m_observersMutex);
    m_observers.push_back(&observer);
}

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
