#ifndef FINALLY_H
#define FINALLY_H

#include <dsnutil/compiler_features.h>
#include <functional>

namespace dsn {

class finally {
    std::function<void(void)> m_functor;

public:
    finally() dsnutil_cpp_DELETED_FUNCTION;
    finally(const finally& other) dsnutil_cpp_DELETED_FUNCTION;
    finally(const std::function<void(void)>& functor)
        : m_functor(functor)
    {
    }
    ~finally() { m_functor(); }
};
}

#endif // FINALLY_H
