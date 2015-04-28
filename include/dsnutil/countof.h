#ifndef DSNUTIL_COUNTOF_H
#define DSNUTIL_COUNTOF_H

#include <cstddef>

namespace dsn {
template <class T, size_t N>
constexpr size_t countof(const T (&)[N])
{
    return N;
}
}

#endif // COUNTOF_H
