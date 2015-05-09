#ifndef DSNUTIL_COUNTOF_H
#define DSNUTIL_COUNTOF_H

#include <cstddef>
#include <dsnutil/compiler_features.h>

namespace dsn {
template <class T, size_t N>
#ifdef dsnutil_cpp_COMPILER_CXX_CONSTEXPR
constexpr size_t countof(const T (&)[N])
#else
size_t countof(const T (&)[N])
#endif
{
    return N;
}
}

#endif // COUNTOF_H
