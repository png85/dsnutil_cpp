#ifndef FINALLY_H
#define FINALLY_H

#include <functional>

namespace dsn {

class finally
{
    std::function<void(void)> m_functor;
public:
    finally() = delete;
    finally(const finally& other) = delete;
    finally(const std::function<void(void)>& functor) : m_functor(functor) {}
    ~finally() { m_functor(); }
};

}

#endif // FINALLY_H
