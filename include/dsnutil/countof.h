#ifndef DSNUTIL_COUNTOF_H
#define DSNUTIL_COUNTOF_H

#include <cstddef>

namespace dsn {
template <class T, size_t N>
#ifdef HAVE_CXX11_CONSTEXPR
constexpr size_t countof(const T (&)[N])
#else
size_t countof(const T (&)[N])
#endif
{
    return N;
}
}

#endif // COUNTOF_H
