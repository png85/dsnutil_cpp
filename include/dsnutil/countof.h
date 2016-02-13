#pragma once

#include <type_traits>

namespace dsn {
namespace detail {
    /// \brief Implementation of dsn::countof
    template <typename Tin> constexpr std::size_t countof()
    {
        using T = typename std::remove_reference<Tin>::type;

        static_assert(std::is_array<T>::value, "countof() only works with array types!");
        static_assert(std::extent<T>::value > 0, "countof() only works with arrays of known length");

        return std::extent<T>::value;
    }
}

/// \brief Determine array size at compile time
///
/// This metaprogramming helper can be used to determine the size of T[constant_number] and T[]
/// during compile time so it can be used for loop counters etc.
///
/// \tparam T array type for which the length shall be determined
///
/// \return Array length as number of elements
template <class T> constexpr std::size_t countof(const T(&)) { return detail::countof<T>(); }
}

