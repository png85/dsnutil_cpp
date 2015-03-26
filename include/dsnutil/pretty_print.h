#ifndef PRETTY_PRINT_HH
#define PRETTY_PRINT_HH

#include <iostream>
#include <ostream>
#include <string>


namespace dsn {

namespace pretty_print {

template<typename T> void print(std::ostream& o, const T& t) {
    o << t;
}

void print(std::ostream &o, const std::string& s) {
    o << '"' << s << '"';
}

template <typename kT, typename vT> void print(std::ostream& o, const std::pair<kT, vT> p) {
    o << '{';
    print(o, p.first);
    o << " : ";
    print(o, p.second);
    o << '}';
}


template <typename I> void print_loop(std::ostream&o, I b, I e) {
    o << '[';
    if (b == e) {
        o << ']';
        return;
    }

    --e;

    for(; b!=e; ++b) {
        print(o, *b);
        o << ", ";
    }

    print (o, *b);
    o << ']';
}

template <typename T> void p(T const& t, std::ostream& o=std::cout) {
    o << t << '\n' << std::flush;
}

} // namespace pretty_print
} // namespace dsn

namespace std {

template <typename T, std::size_t N>
typename std::enable_if<std::is_array<T[]>::value && !std::is_same<T, char>::value && !std::is_same<T, wchar_t>::value, std::ostream &>::type
operator<< (std::ostream &o, T const (&a)[N])
{
    dsn::pretty_print::print_loop(o, &a[0], &a[N]);
    return o;
}


template <typename C, typename I= typename C::const_iterator>
typename std::enable_if<!std::is_same<C, std::string>::value, std::ostream &>::type
operator<< (std::ostream &o, C const &c)
{
    dsn::pretty_print::print_loop(o, c.begin(), c.end());
    return o;
}


} // namespace std

#endif // PRETTY_PRINT_HH
