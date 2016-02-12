#ifndef DSNUTIL_MAKE_REVERSE_H
#define DSNUTIL_MAKE_REVERSE_H

namespace dsn {
namespace reverse {
    /// \brief Reverse iterator adapter
    ///
    /// This metaprogramming helper swaps the begin/end iterators for a container so it is
    /// traversed in reverse direction.
    template <class T> class adapter {
    public:
        adapter(T& container)
            : m_container(container)
        {
        }

        typename T::reverse_iterator begin() { return m_container.rbegin(); }

        typename T::reverse_iterator end() { return m_container.rend(); }

    private:
        /// \brief Container that we're operating on
        T& m_container;
    };

    /// \brief Reverse iterator adapter (const version)
    ///
    /// \see adapter
    template <class T> class const_adapter {
    public:
        const_adapter(const T& container)
            : m_container(container)
        {
        }

        typename T::const_reverse_iterator begin() { return m_container.rbegin(); }

        typename T::const_reverse_iterator end() { return m_container.rend(); }

    private:
        /// \brief Container that we're operating on
        const T& m_container;
    };
} // namespace dsn::reverse

template <class T> reverse::adapter<T> make_reverse(T& container) { return reverse::adapter<T>(container); }

template <class T> reverse::const_adapter<T> make_reverse(const T& container)
{
    return reverse::const_adapter<T>(container);
}

} // namespace dsn

#endif // DSNUTIL_MAKE_REVERSE_H
