#pragma once

#include <algorithm>
#include <dsnutil/reference_counted.hpp>

namespace dsn {
/// \brief Intrusive smart pointer for \p reference_counted objects
///
/// This provides a smart pointer for objects implementing the \a reference_counted interface.
template <typename T> class intrusive_ptr {
    static_assert(is_reference_counted<T>::value, "intrusive_ptr<T> only works with observable types!");

protected:
    /// \brief Raw-pointer to the managed object
    T* m_ptr{ nullptr };

public:
    intrusive_ptr() = default;
    ~intrusive_ptr();
    intrusive_ptr(T* obj);
    intrusive_ptr(T& obj);
    intrusive_ptr(const intrusive_ptr& other);
    intrusive_ptr& operator=(const intrusive_ptr& other);
    intrusive_ptr(intrusive_ptr&& other) noexcept;
    intrusive_ptr& operator=(intrusive_ptr&& other) noexcept;

    void reset(T* obj = nullptr);
    void reset(T& obj);

    T* get();

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

    operator bool() const;

    void swap(intrusive_ptr& other) noexcept;
};

/// \brief Destroy intrusive_ptr
///
/// This decreases the reference count of the managed object and possibly triggers its destruction.
template <typename T> intrusive_ptr<T>::~intrusive_ptr()
{
    if (m_ptr != nullptr) {
        m_ptr->unref();
    }
}

/// \brief Create from existing pointer
///
/// \param obj Pointer to the \a reference_counted object that shall be managed by this pointer
template <typename T>
intrusive_ptr<T>::intrusive_ptr(T* obj)
    : m_ptr(obj)
{
    if (m_ptr != nullptr) {
        m_ptr->ref();
    }
}

/// \brief Create from refrence
///
/// \param obj Reference to the \a reference_counted object that shall be managed by this pointer
template <typename T>
intrusive_ptr<T>::intrusive_ptr(T& obj)
    : m_ptr(&obj)
{
    m_ptr->ref();
}

/// \brief Copy-construct from another intrusive_ptr
///
/// \param other intrusive_ptr that shall be copied
template <typename T>
intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<T>& other)
    : m_ptr(other.m_ptr)
{
    if (m_ptr) {
        m_ptr->ref();
    }
}

/// \brief Copy-assign from another intrusive_ptr
///
/// \param other intrusive_ptr that shall be copied
///
/// \return reference to this object after the assignment
template <typename T> intrusive_ptr<T>& intrusive_ptr<T>::operator=(const intrusive_ptr<T>& other)
{
    reset(other.m_ptr);
    return *this;
}

/// \brief Move-construct from another intrusive_ptr
///
/// Moves the object reference from \a other into this pointer.
///
/// \note \a other will be in an invalid state after this operation
///
/// \param other other intrusive_ptr that shall be moved into this one
template <typename T> intrusive_ptr<T>::intrusive_ptr(intrusive_ptr<T>&& other) noexcept : m_ptr(other.m_ptr)
{
    other.m_ptr = nullptr;
}

/// \brief Move-assign from another intrusive_ptr
///
/// Moves the object reference from \a other into this pointer.
///
/// \note \a other will be in an invalid state after this operation
///
/// \param other other intrusive_ptr that shall be moved into this one
///
/// \return reference to this object after the assignment
template <typename T> intrusive_ptr<T>& intrusive_ptr<T>::operator=(intrusive_ptr<T>&& other) noexcept
{
    reset();
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
    return *this;
}

/// \brief Reset to new pointer
///
/// Resets the pointed-to object to \a obj. If the previous object's reference count reaches zero it will
/// be destroyed.
///
/// \param obj Pointer to the new object managed by this pointer
template <typename T> void intrusive_ptr<T>::reset(T* obj)
{
    if (obj != nullptr) {
        obj->ref();
    }

    if (m_ptr != nullptr) {
        m_ptr->unref();
    }

    m_ptr = obj;
}

/// \brief Reset to new reference
///
/// Resets the pointed-to object to \a obj. If the previous object's reference count reaches zero it will
/// be destroyed.
///
/// \param obj Reference to the new object managed by this pointer
template <typename T> void intrusive_ptr<T>::reset(T& obj)
{
    obj.ref();

    if (m_ptr != nullptr) {
        m_ptr->unref();
    }

    m_ptr = &obj;
}

/// \brief Get raw pointer
///
/// \return Raw pointer to the managed object or nullptr in case the pointer is invalid
template <typename T> T* intrusive_ptr<T>::get() { return m_ptr; }

/// \brief Dereference operator
template <typename T> T& intrusive_ptr<T>::operator*() { return *m_ptr; }

/// \brief Dereference operator (const)
template <typename T> const T& intrusive_ptr<T>::operator*() const { return *m_ptr; }

/// \brief Member access operator
template <typename T> T* intrusive_ptr<T>::operator->() { return m_ptr; }

/// \brief Member access operator (const)
template <typename T> const T* intrusive_ptr<T>::operator->() const { return m_ptr; }

/// \brief Conversion to bool
///
/// \return true if the pointer is valid or false if it doesn't point to any object
template <typename T> intrusive_ptr<T>::operator bool() const { return (m_ptr != nullptr); }

/// \brief Swap two intrusive_ptr's
///
/// This is used to implement \p std::swap for two intrusive_ptr instances
///
/// \param other Other intrusive_ptr that shall be swapped with this one
template <typename T> void intrusive_ptr<T>::swap(intrusive_ptr<T>& other) noexcept { std::swap(other.m_ptr, m_ptr); }
}
