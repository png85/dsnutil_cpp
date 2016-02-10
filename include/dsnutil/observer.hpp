#pragma once

#include <type_traits>

namespace dsn {

class observable;

class observer {
    friend class observable;
    virtual void observable_destroyed(observable&) = 0;
};

template <typename T> struct is_observer {
    static const bool value = std::is_base_of<observable, T>::value;
};
}
