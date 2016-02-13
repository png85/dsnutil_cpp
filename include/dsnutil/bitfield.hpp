#pragma once

#include <algorithm>
#include <bitset>
#include <climits>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <typeinfo>

namespace dsn {

template <typename T> class bitfield {
    /// \brief Bitfield-esque wrapper for enum values
    ///
    /// \note This currently assumes that sizeof(char) = 8bits and will most likely break if otherwise, caveat haxor!
    static_assert(
        std::is_integral<typename std::underlying_type<T>::type>::value, "bitfield<T> only works with integral types!");

public:
    using type = typename std::underlying_type<T>::type;

    bitfield();
    bitfield(T value);
    bitfield(std::initializer_list<T> initializer);

    bitfield(const bitfield&) = default;
    bitfield& operator=(const bitfield&) = default;

    bitfield(bitfield&&) noexcept;
    bitfield& operator=(bitfield&&) noexcept;

    friend bitfield operator|(const bitfield& field, T flag)
    {
        auto result = field;
        result.set(flag);
        return result;
    }

    friend bitfield operator|(T flag, const bitfield& field)
    {
        auto result = field;
        result.set(flag);
        return result;
    }

    bitfield operator!() const;

    bitfield& operator|=(T flag);
    bitfield& operator&=(T flag);
    bitfield& operator^=(T flag);

    bitfield& operator|=(const bitfield& field);
    bitfield& operator&=(const bitfield& field);
    bitfield& operator^=(const bitfield& field);

    bool operator==(const bitfield& field) const;
    bool operator!=(const bitfield& field) const;

    void set(T flag);
    bool is_set(T flag) const;

    void clear();
    void clear(T flag);

    T value() const;
    type raw_value() const;

    /// \brief Number of bits required to store underlying type
    static constexpr size_t num_bits() { return sizeof(type) * CHAR_BIT; }

    /// \brief Type alias for a bitset that can hold all possible enum values
    using bitset_type = std::bitset<num_bits()>;

    /// \brief Output bitfield on ostream
    friend std::ostream& operator<<(std::ostream& stream, const bitfield& field)
    {
        std::stringstream buf;
        bitfield<T>::bitset_type bitset(field.m_value);
        buf << "bitset<" << typeid(T).name() << " [" << typeid(bitfield<T>::type).name() << "]>: " << bitset.to_string()
            << " (= 0x" << std::hex << field.m_value << ", " << field.m_value << ")";

        return stream << buf.str();
    }

private:
    type m_value;
};

template <typename T>
bitfield<T>::bitfield()
    : m_value(0)
{
}

template <typename T>
bitfield<T>::bitfield(T value)
    : m_value(0)
{
    set(value);
}

template <typename T>
bitfield<T>::bitfield(std::initializer_list<T> initializer)
    : m_value(0)
{
    for (T bit : initializer)
        set(bit);
}

template <typename T> bitfield<T>::bitfield(bitfield<T>&& other) noexcept : m_value(other.m_value) { other.clear(); }

template <typename T> bitfield<T>& bitfield<T>::operator=(bitfield<T>&& other) noexcept
{
    m_value = other.m_value;
    other.clear();
    return *this;
}

template <typename T> bitfield<T> bitfield<T>::operator!() const
{
    bitfield<T> result = *this;

    result.m_value = ~result.m_value;

    return result;
}

template <typename T> bitfield<T>& bitfield<T>::operator|=(T flag)
{
    m_value |= static_cast<type>(flag);
    return *this;
}

template <typename T> bitfield<T>& bitfield<T>::operator&=(T flag)
{
    m_value &= static_cast<type>(flag);
    return *this;
}

template <typename T> bitfield<T>& bitfield<T>::operator^=(T flag)
{
    m_value ^= static_cast<type>(flag);
    return *this;
}

template <typename T> bitfield<T>& bitfield<T>::operator|=(const bitfield<T>& field)
{
    m_value |= field.m_value;
    return *this;
}

template <typename T> bitfield<T>& bitfield<T>::operator&=(const bitfield<T>& field)
{
    m_value &= field.m_value;
    return *this;
}

template <typename T> bitfield<T>& bitfield<T>::operator^=(const bitfield<T>& field)
{
    m_value ^= field.m_value;
    return *this;
}

template <typename T> bool bitfield<T>::operator==(const bitfield<T>& field) const { return m_value == field.m_value; }

template <typename T> bool bitfield<T>::operator!=(const bitfield<T>& field) const { return m_value != field.m_value; }

template <typename T> void bitfield<T>::set(T flag) { m_value |= static_cast<type>(flag); }

template <typename T> bool bitfield<T>::is_set(T flag) const { return (m_value & static_cast<type>(flag)) != 0; }

template <typename T> void bitfield<T>::clear() { m_value = static_cast<type>(0); }

template <typename T> void bitfield<T>::clear(T flag) { m_value &= static_cast<type>(flag); }

template <typename T> T bitfield<T>::value() const { return static_cast<T>(m_value); }

template <typename T> typename bitfield<T>::type bitfield<T>::raw_value() const { return m_value; }
}
