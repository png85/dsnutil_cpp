#ifndef DSNUTIL_COUNTOF_H
#define DSNUTIL_COUNTOF_H

#include <cstddef>
#include <dsnutil/compiler_features.h>

namespace dsn {
template <class T, size_t N>
dsnutil_cpp_CONSTEXPR size_t countof(const T (&)[N])
{
    return N;
}
}

#endif // COUNTOF_H
