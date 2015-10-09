#ifndef FINALLY_H
#define FINALLY_H

#include <dsnutil/compiler_features.h>
#include <functional>

namespace dsn {

/// \brief Final execution block
///
/// This utility class can be used to implement a block of code which is guaranteed to
/// be executed upon leaving a function by taking a lambda expression or other callable
/// interface and executing it when the object's instance goes out of scope.
///
/// Using this class will provide a functionality similar to Java's \a finally{} blocks
class finally {
    /// \brief Functor to execute at object destruction
    std::function<void(void)> m_functor;

public:
    finally() dsnutil_cpp_DELETED_FUNCTION;
    finally(const finally& other) dsnutil_cpp_DELETED_FUNCTION;

    /// \brief Initialize finally block
    ///
    /// Initializes \a m_functor from the given reference so that it gets executed when the
    /// instance of this object goes out of scope.
    ///
    /// \param functor Reference to a callable object to execute upon leaving the current scope
    finally(const std::function<void(void)>& functor)
        : m_functor(functor)
    {
    }

    /// \brief Execute functor
    ///
    /// Executes the functor in \a m_functor when the instance of this class goes out of scope
    ~finally() { m_functor(); }
};
}

#endif // FINALLY_H
