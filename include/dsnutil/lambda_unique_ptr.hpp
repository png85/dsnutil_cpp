#pragma once

#include <functional>
#include <memory>

namespace dsn {

/// \brief Utility type that extends unique_ptr with lambda deleters
///
/// This can be used to define an unique_ptr with a lambda function that gets executed as the
/// pointer's custom deleter instead of calling delete. The main use for this is to be able
/// to work with C functions that return malloc'd pointers in a RAII way but technically it can
/// be used with just about any pointer-based resource that needs cleanup.
template <typename T> using lambda_unique_ptr = std::unique_ptr<T, std::function<void(T*)> >;
}
