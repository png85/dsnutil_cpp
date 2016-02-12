#include <iostream>

#include <dsnutil/reference_counted.hpp>
#include <dsnutil/throwing_assert.h>

/// \brief Increase reference count
///
/// This increases the object's reference count and puts it one step further away from suicide.
void dsn::reference_counted::ref() const { m_reference_count++; }

/// \brief Decrease reference count
///
/// This decreases the reference count of the object. If the reference count its zero and \a no_delete
/// is \a false the object will commit suicide (= delete itself)
void dsn::reference_counted::unref(bool no_delete) const
{
    assert(m_reference_count.load() >= 1);

    auto value = --m_reference_count;
    if (!no_delete && value == 0) {
        delete this;
    }
}

/// \brief Obtain current reference count
///
/// \return Current reference count of this object
dsn::reference_counted::refcount_t dsn::reference_counted::reference_count() const { return m_reference_count.load(); }
