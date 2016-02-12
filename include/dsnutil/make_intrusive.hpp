#pragma once

#include <dsnutil/intrusive_ptr.hpp>

namespace dsn {

template <typename T, typename... Args> intrusive_ptr<T> make_intrusive(Args&&... args)
{
    static_assert(is_reference_counted<T>::value, "make_intrusive<T> only works for reference_counted types!");
    return dsn::intrusive_ptr<T>(new T(std::forward<Args>(args)...));
}
}
