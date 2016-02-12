#pragma once

#include <atomic>
#include <dsnutil/observable.hpp>
#include <dsnutil/observer.hpp>

namespace dsn {

/// \brief Smart pointer for \p observable objects
///
/// This provides a smart pointer that can observe the livetime of an \p observable object. When the object
/// gets destroyed it will reset itself to \a nullptr so it's possible to check whether the pointed-to object
/// still exists.
///
/// \note This pointer doesn't perform any memory management by itself and is to be considered non-owning!
///
/// \tparam T Sub-class of \p observable that shall be watched by this smart pointer
///
/// \see observable
/// \implements observer
template <typename T> class observing_ptr : public observer {
    static_assert(is_observable<T>::value, "observing_ptr<T> only works with subclasses of dsn::observable!");

private:
    /// \brief Pointer to observed object
    std::atomic<T*> m_object{ nullptr };

    virtual void observable_destroyed(observable& o) override;

public:
    observing_ptr() { m_object = nullptr; }

    observing_ptr(T* obj);

    observing_ptr(T& obj);

    ~observing_ptr();

    observing_ptr(const observing_ptr& other);

    observing_ptr& operator=(const observing_ptr& other);

    observing_ptr(observing_ptr&& other) noexcept;

    observing_ptr& operator=(observing_ptr&& other) noexcept;

    void reset(T* obj = nullptr);

    void reset(T& obj);

    T* get();

    T& operator*();

    const T& operator*() const;

    T* operator->();

    const T* operator->() const;

    operator bool() const;

    void swap(observing_ptr& other) noexcept;

    bool operator==(const observing_ptr& other) const;
    bool operator!=(const observing_ptr& other) const;
};

/// \brief Construct from pointer
///
/// \param obj Pointer to the \a observable that shall be tracked
template <typename T>
observing_ptr<T>::observing_ptr(T* obj)
    : m_object(obj)
{
    if (m_object != nullptr) {
        m_object.load()->addObserver(*this);
    }
}

/// \brief Construct from reference
///
/// \param obj Reference to the \a observable that shall be tracked
template <typename T>
observing_ptr<T>::observing_ptr(T& obj)
    : m_object(&obj)
{
    m_object.load()->addObserver(*this);
}

/// \brief Remove observing pointer
///
/// Unregisters ourself from the pointed-to \a observable
template <typename T> observing_ptr<T>::~observing_ptr()
{
    if (m_object != nullptr) {
        m_object.load()->removeObserver(*this);
    }
}

/// \brief Copy-construct from other observing_ptr
///
/// \param other Reference to another \a observing_ptr that shall be copied
template <typename T>
observing_ptr<T>::observing_ptr(const observing_ptr<T>& other)
    : m_object(other.m_object.load())
{
    if (m_object != nullptr) {
        m_object.load()->addObserver(*this);
    }
}

/// \brief Copy-assign from other observing_ptr
///
/// \param other Reference to another \a observing_ptr that shall be copied
///
/// \return reference to this object after the assignment
template <typename T> observing_ptr<T>& observing_ptr<T>::operator=(const observing_ptr<T>& other)
{
    reset(other.m_object);
    return *this;
}

/// \brief Move-construct from another \a observing_ptr
///
/// \param other rvalue reference to another \a observing_ptr that we shall take over
template <typename T>
observing_ptr<T>::observing_ptr(observing_ptr<T>&& other) noexcept : m_object(other.m_object.load())
{
    if (m_object != nullptr) {
        m_object.load()->moveObserver(other, *this);
    }
    other.m_object = nullptr;
}

/// \brief Move-assign from another observing_ptr
///
/// \param other rvalue reference to another \a observing_ptr that we shall take over
///
/// \return reference to this object after the assignment operation
template <typename T> observing_ptr<T>& observing_ptr<T>::operator=(observing_ptr<T>&& other) noexcept
{
    reset();
    m_object = other.m_object.load();
    if (m_object != nullptr) {
        m_object.load()->moveObserver(other, *this);
    }
    other.m_object = nullptr;

    return *this;
}

/// \brief Reset from pointer
///
/// Sets the pointed-to object to \a obj and registers/unregisters ourself as an observer as needed.
///
/// \param obj Pointer to the new \a observable that shall be tracked
template <typename T> void observing_ptr<T>::reset(T* obj)
{
    if (obj != nullptr) {
        obj->addObserver(*this);
    }
    if (m_object != nullptr) {
        m_object.load()->removeObserver(*this);
    }

    m_object = obj;
}

/// \brief Reset from reference
///
/// Sets the pointed-to object to \a obj and registers/unregisters ourself as an observer as needed.
///
/// \param obj Reference to the new \a observable that shall be tracked
template <typename T> void observing_ptr<T>::reset(T& obj)
{
    obj.addObserver(*this);
    if (m_object != nullptr) {
        m_object.load()->removeObserver(*this);
    }
    m_object = &obj;
}

/// \brief Access by raw pointer
template <typename T> T* observing_ptr<T>::get() { return m_object.load(); }

/// \brief Reference access
template <typename T> T& observing_ptr<T>::operator*() { return *m_object.load(); }

/// \brief Constant reference access
template <typename T> const T& observing_ptr<T>::operator*() const { return *m_object.load(); }

/// \brief Dereference operator
template <typename T> T* observing_ptr<T>::operator->() { return m_object.load(); }

/// \brief Const dereference operator
template <typename T> const T* observing_ptr<T>::operator->() const { return m_object.load(); }

/// \brief Cast to boolean value
///
/// \return true if the object we're pointing to is still alive or false if it has been destroyed
template <typename T> observing_ptr<T>::operator bool() const { return (m_object != nullptr); }

/// \brief Helper for \p std::swap
///
/// This is used to implement \p std::swap with two observing_ptr's. It swaps the underlying
/// raw pointers and updates the pointed-to \a observable as needed.
///
/// \param other observing_ptr that shall be swapped with this one
template <typename T> void observing_ptr<T>::swap(observing_ptr<T>& other) noexcept
{
    if (m_object != nullptr) {
        m_object.load()->moveObserver(*this, other);
    }

    if (other.m_object) {
        other.m_object.load()->moveObserver(other, *this);
    }

    std::swap(m_object, other.m_object);
}

/// \brief Comparison operator (equality)
template <typename T> bool observing_ptr<T>::operator==(const observing_ptr<T>& other) const
{
    return (m_object == other.m_object);
}

/// \brief Comparison operator (inequality)
template <typename T> bool observing_ptr<T>::operator!=(const observing_ptr<T>& other) const
{
    return (m_object != other.m_object);
}

/// \brief Callback for \a observable
///
/// This is called by the watched \a observable when it is deleted.
///
/// \param o Reference to the destroyed \a observable
template <typename T> void observing_ptr<T>::observable_destroyed(observable& o) { m_object = nullptr; }
}
