#ifndef DSN_MAP_HH
#define DSN_MAP_HH

#include <dsnutil/map_sort.h>
#include <functional>
#include <map>
#include <vector>

namespace dsn {

/**
 * @brief Extract keys from a map
 *
 * @tparam kT Key data type
 * @tparam vT Value data type
 *
 * @param in Map from which the keys shall be extracted
 * @param out Reference to a vector<kT> that shall hold the extracted keys
 */
template <typename kT, typename vT>
void map_keys(const std::map<kT, vT>& in, std::vector<kT>& out)
{
    out.clear();
    out.resize(in.size());
    std::transform(in.begin(), in.end(), out.begin(), [&](std::pair<kT, vT> p) { return p.first; });
}

/**
 * @brief Obtain vector of keys from a map
 *
 * @tparam kT Key data type
 * @tparam vT Value data type
 *
 * @param in Map from which the keys shall be extracted
 * @return vector<kT> that contains all keys for in
 */
template <typename kT, typename vT>
std::vector<kT> map_keys(const std::map<kT, vT>& in)
{
    std::vector<kT> out;
    map_keys(in, out);
    return out;
}

/**
 * @brief Sort map keys by their corresponding value
 *
 * Sorts the keys of the given input map by their corresponding values in ascending order and
 * stores them in a vector.
 *
 * @tparam kT Key data type
 * @tparam vT Value data type
 *
 * @param in Map that shall be sorted
 * @param out Reference to a vector<kT> that shall hold the sorted map keys
 *
 * @note vT needs to implement operator < for this template to work!
 */
template <typename kT, typename vT>
void map_sort(std::map<kT, vT>& in, std::vector<kT>& out)
{
    map_keys(in, out);
    std::sort(out.begin(), out.end(), [&](kT a, kT b) { return in[a] < in[b]; });
}

template <typename kT, typename vT>
std::vector<kT> map_sort(std::map<kT, vT>& in)
{
    std::vector<kT> keys = map_keys(in);
    map_sort(in, keys);
    return keys;
}

/**
 * @brief Sort map keys with custom comparator function
 *
 * Sorts the keys of the given input map in the order specified by the supplied comparator function.
 *
 * @tparam kT Key data type
 * @tparam vT Value data type
 *
 * @param in Map that shall be sorted
 * @param out Reference to a vector<kT> that shall hold the sorted map keys
 * @param comparator Custom comparator function for sorting the map
 */
template <typename kT, typename vT>
void map_sort(std::map<kT, vT>& in, std::vector<kT>& out, const std::function<bool(kT, kT)>& comparator)
{
    map_keys(in, out);
    std::sort(out.begin(), out.end(), comparator);
}

template <typename kT, typename vT>
std::vector<kT> map_sort(std::map<kT, vT>& in, const std::function<bool(kT, kT)>& comparator)
{
    std::vector<kT> keys = map_keys(in);
    map_sort(in, keys, comparator);
    return keys;
}
}

#endif // DSN_MAP_HH
