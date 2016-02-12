#pragma once

#include <algorithm>
#include <dsnutil/reference_counted.hpp>

namespace dsn {
template <typename T> class intrusive_ptr {
    static_assert(is_reference_counted<T>::value, "intrusive_ptr<T> only works with observable types!");

protected:
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

template <typename T> intrusive_ptr<T>::~intrusive_ptr()
{
    if (m_ptr != nullptr) {
        m_ptr->unref();
    }
}

template <typename T>
intrusive_ptr<T>::intrusive_ptr(T* obj)
    : m_ptr(obj)
{
    if (m_ptr != nullptr) {
        m_ptr->ref();
    }
}

template <typename T>
intrusive_ptr<T>::intrusive_ptr(T& obj)
    : m_ptr(&obj)
{
    m_ptr->ref();
}

template <typename T>
intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<T>& other)
    : m_ptr(other.m_ptr)
{
    if (m_ptr) {
        m_ptr->ref();
    }
}

template <typename T> intrusive_ptr<T>& intrusive_ptr<T>::operator=(const intrusive_ptr<T>& other)
{
    reset(other.m_ptr);
    return *this;
}

template <typename T> intrusive_ptr<T>::intrusive_ptr(intrusive_ptr<T>&& other) noexcept : m_ptr(other.m_ptr)
{
    other.m_ptr = nullptr;
}

template <typename T> intrusive_ptr<T>& intrusive_ptr<T>::operator=(intrusive_ptr<T>&& other) noexcept
{
    reset();
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
    return *this;
}

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

template <typename T> void intrusive_ptr<T>::reset(T& obj)
{
    obj.ref();

    if (m_ptr != nullptr) {
        m_ptr->unref();
    }

    m_ptr = &obj;
}

template <typename T> T* intrusive_ptr<T>::get() { return m_ptr; }

template <typename T> T& intrusive_ptr<T>::operator*() { return *m_ptr; }

template <typename T> const T& intrusive_ptr<T>::operator*() const { return *m_ptr; }

template <typename T> T* intrusive_ptr<T>::operator->() { return m_ptr; }

template <typename T> const T* intrusive_ptr<T>::operator->() const { return m_ptr; }

template <typename T> intrusive_ptr<T>::operator bool() const { return (m_ptr != nullptr); }

template <typename T> void intrusive_ptr<T>::swap(intrusive_ptr<T>& other) noexcept { std::swap(other.m_ptr, m_ptr); }
}
