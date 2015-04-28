#ifndef DSNUTIL_MAKE_REVERSE_H
#define DSNUTIL_MAKE_REVERSE_H

namespace dsn {
namespace reverse {
    template <class T>
    class adapter {
    public:
        adapter(T& container)
            : m_container(container)
        {
        }

        typename T::reverse_iterator begin()
        {
            return m_container.rbegin();
        }

        typename T::reverse_iterator end()
        {
            return m_container.rend();
        }

    private:
        T& m_container;
    };

    template <class T>
    class const_adapter {
    public:
        const_adapter(const T& container)
            : m_container(container)
        {
        }

        typename T::const_reverse_iterator begin()
        {
            return m_container.rbegin();
        }

        typename T::const_reverse_iterator end()
        {
            return m_container.rend();
        }

    private:
        const T& m_container;
    };
} // namespace dsn::reverse

template <class T>
reverse::adapter<T> make_reverse(T& container)
{
    return reverse::adapter<T>(container);
}

template <class T>
reverse::const_adapter<T> make_reverse(const T& container)
{
    return reverse::const_adapter<T>(container);
}

} // namespace dsn

#endif // DSNUTIL_MAKE_REVERSE_H
