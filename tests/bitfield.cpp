#define BOOST_TEST_MODULE "dsn::bitfield"

#include <dsnutil/bitfield.hpp>
#include <iomanip>
#include <iostream>

#include <boost/test/unit_test.hpp>

enum class Test : unsigned short {
    None = 0,
    Foo = (1 << 0),
    Bar = (1 << 1),
    Bla = (1 << 2),
};

std::ostream& operator<<(std::ostream& stream, const Test& t)
{
    using T = typename std::underlying_type<Test>::type;
    constexpr auto bitset_size = sizeof(T) * CHAR_BIT;
    using binary_type = std::bitset<bitset_size>;
    auto value = static_cast<T>(t);

    return stream << binary_type(value).to_string() << " (" << value << ")";
}

using field = dsn::bitfield<Test>;

BOOST_AUTO_TEST_CASE(mp_helpers)
{
    std::cout << "dsn::bitfield<Test> requires " << field::num_bits() << "bits of storage." << std::endl;
    BOOST_CHECK(field::num_bits() == sizeof(std::underlying_type<Test>::type) * CHAR_BIT);
}

BOOST_AUTO_TEST_CASE(default_construction)
{
    std::cout << "Test enum values are: " << std::endl
              << "\tNone= " << Test::None << std::endl
              << "\tFoo = " << Test::Foo << std::endl
              << "\tBar = " << Test::Bar << std::endl
              << "\tBla = " << Test::Bla << std::endl;

    field f;
    std::cout << "-- Testing default construction" << std::endl;
    std::cout << "bitfield() = " << f << std::endl;

    BOOST_CHECK(f.value() == Test::None);
}

BOOST_AUTO_TEST_CASE(construct_with_value)
{
    std::cout << "-- Testing construction with value" << std::endl;
    field f(Test::Bla);
    std::cout << "bitfield(" << Test::Bla << ") = " << f << std::endl;

    BOOST_CHECK(f.value() == Test::Bla);
}

BOOST_AUTO_TEST_CASE(construct_with_initializer_list)
{
    std::cout << "-- Testing construction with initializer list" << std::endl;
    field f({ Test::Foo, Test::Bar });
    std::cout << "bitfield({" << Test::Foo << "," << Test::Bar << "}) = " << f << std::endl;

    BOOST_CHECK(f.is_set(Test::Foo) == true);
    BOOST_CHECK(f.is_set(Test::Bar) == true);
}

BOOST_AUTO_TEST_CASE(copy_construct)
{
    std::cout << "-- Testing copy construction" << std::endl;
    field f1(Test::Bla);
    std::cout << "f1 = " << f1 << std::endl;
    BOOST_CHECK(f1.is_set(Test::Bla) == true);

    field f2(f1);
    std::cout << "f2 = " << f2 << std::endl;
    BOOST_CHECK(f1.value() == f2.value());
}

BOOST_AUTO_TEST_CASE(copy_assign)
{
    std::cout << "-- Testing copy assignment" << std::endl;
    field f1(Test::Bla);
    std::cout << "f1 = " << f1 << std::endl;
    BOOST_CHECK(f1.is_set(Test::Bla) == true);

    field f2;
    std::cout << "f2 = " << f2 << " (before assignment)" << std::endl;
    BOOST_CHECK(f2.is_set(Test::Bla) == false);

    f2 = f1;
    std::cout << "f2 = " << f2 << " (after assignment)" << std::endl;
    BOOST_CHECK(f2.is_set(Test::Bla) == true);
}

BOOST_AUTO_TEST_CASE(move_construct)
{
    field f1(Test::Bla);
    std::cout << "f1 = " << f1 << " (before move)" << std::endl;
    BOOST_CHECK(f1.is_set(Test::Bla) == true);

    field f2(std::move(f1));
    std::cout << "f2 = " << f2 << std::endl << "f1 = " << f1 << " (after move)" << std::endl;
    BOOST_CHECK(f2.is_set(Test::Bla) == true);
    BOOST_CHECK(f1.is_set(Test::Bla) == false);
}

BOOST_AUTO_TEST_CASE(move_assign)
{
    std::cout << "-- Testing move assignment" << std::endl;
    field f1(Test::Bla);
    std::cout << "f1 = " << f1 << " (before move)" << std::endl;
    BOOST_CHECK(f1.is_set(Test::Bla) == true);

    field f2;
    std::cout << "f2 = " << f2 << " (before move)" << std::endl;
    BOOST_CHECK(f2.is_set(Test::Bla) == false);

    f2 = std::move(f1);
    std::cout << "f1 = " << f1 << " (after move)" << std::endl << "f2 = " << f2 << " (after move)" << std::endl;
    BOOST_CHECK(f2.is_set(Test::Bla) == true);
    BOOST_CHECK(f1.is_set(Test::Bla) == false);
}

BOOST_AUTO_TEST_CASE(set_flag)
{
    std::cout << "-- Testing set(flag)" << std::endl;
    field f;
    std::cout << "f = " << f << " (before set)" << std::endl;
    BOOST_CHECK(f.is_set(Test::Bla) == false);

    f.set(Test::Bla);
    std::cout << "f = " << f << " (after set)" << std::endl;
    BOOST_CHECK(f.is_set(Test::Bla) == true);
}

BOOST_AUTO_TEST_CASE(clear_flags)
{
    std::cout << "-- Testing clear without mask" << std::endl;
    field f(Test::Bla);
    std::cout << "f = " << f << " (before clear)" << std::endl;
    BOOST_CHECK(f.is_set(Test::Bla) == true);

    f.clear();
    std::cout << "f = " << f << " (after clear)" << std::endl;
    BOOST_CHECK(f.is_set(Test::Bar) == false);
}

BOOST_AUTO_TEST_CASE(clear_flags_with_mask)
{
    std::cout << "-- Testing clear with mask" << std::endl;
    field f({ Test::Foo, Test::Bla });
    std::cout << "f = " << f << " (before clear)" << std::endl;
    BOOST_CHECK(f.is_set(Test::Foo) and f.is_set(Test::Bla));

    f.clear(Test::Foo);
    std::cout << "f = " << f << " (after clear)" << std::endl;
    BOOST_CHECK(f.is_set(Test::Foo) and !f.is_set(Test::Bla));
}

BOOST_AUTO_TEST_CASE(operator_equality)
{
    field f1(Test::Bla);
    field f2(f1);
    std::cout << "-- Testing comparison operator ==" << std::endl
              << "f1 = " << f1 << std::endl
              << "f2 = " << f2 << std::endl;

    BOOST_CHECK(f1 == f2);
}

BOOST_AUTO_TEST_CASE(operator_inequality)
{
    field f1(Test::Bla);
    field f2(Test::Foo);
    std::cout << "-- Testing comparison operator !=" << std::endl
              << "f1 = " << f1 << std::endl
              << "f2 = " << f2 << std::endl;

    BOOST_CHECK(f1 != f2);
}

BOOST_AUTO_TEST_CASE(raw_value)
{
    field f(Test::Foo);
    const int expected{ static_cast<int>(Test::Foo) };
    std::cout << "-- Testing raw_value()" << std::endl << "f = " << f << ", expected=" << expected << std::endl;
    BOOST_CHECK(f.raw_value() == expected);
}

BOOST_AUTO_TEST_CASE(friend_operator_or_value)
{
    field source(Test::Foo);
    std::cout << "-- Testing operator| with T" << std::endl << "source = " << source << std::endl;

    field f1 = (Test::Bar | source);
    std::cout << "f1 = " << f1 << std::endl;
    BOOST_CHECK((f1.is_set(Test::Foo) == true) and (f1.is_set(Test::Bar) == true));

    field f2 = (source | Test::Bar);
    BOOST_CHECK(f2 == f1);
}

BOOST_AUTO_TEST_CASE(operator_or_bitfield)
{
    field src1(Test::Foo);
    field src2(Test::Bar);
    std::cout << "-- Testing operator| with bitfield<T>" << std::endl
              << "src1 = " << src1 << std::endl
              << "src2 = " << src2 << std::endl;

    field dst1 = src1;
    std::cout << "dst1 = " << dst1 << " (before |=)" << std::endl;
    BOOST_CHECK(dst1 == src1);
    dst1 |= src2;
    std::cout << "dst1 = " << dst1 << " (after |=)" << std::endl;
    BOOST_CHECK((dst1.is_set(Test::Foo) == true) and (dst1.is_set(Test::Bar) == true));
}

BOOST_AUTO_TEST_CASE(operator_and_value)
{
    field f1({ Test::Foo, Test::Bar });
    std::cout << "-- Testing operator &= with T" << std::endl << "f1 = " << f1 << std::endl;
    BOOST_CHECK(f1.is_set(Test::Foo) and f1.is_set(Test::Bar));

    f1 &= Test::Foo;
    std::cout << "f1 = " << f1 << " (after &= " << Test::Foo << ")" << std::endl;
    BOOST_CHECK(f1.is_set(Test::Foo) and !f1.is_set(Test::Bar));

    f1 &= Test::Bla;
    std::cout << "f1 = " << f1 << " (after &= " << Test::Bla << ")" << std::endl;
    BOOST_CHECK(!f1.is_set(Test::Foo));
}

BOOST_AUTO_TEST_CASE(operator_and_bitfield)
{
    field f1({ Test::Foo, Test::Bar });
    std::cout << "-- Testing operator &= with bitfield<T>" << std::endl << "f1 = " << f1 << std::endl;
    BOOST_CHECK(f1.is_set(Test::Foo) and f1.is_set(Test::Bar));

    field f2(Test::Foo);
    std::cout << "f2 = " << f2 << std::endl;
    BOOST_CHECK(f1.is_set(Test::Foo));

    f1 &= f2;
    std::cout << "f1 = " << f1 << " (after &= f2)" << std::endl;
    BOOST_CHECK(f1.is_set(Test::Foo) and !f1.is_set(Test::Bar));

    field f3(Test::Bla);
    std::cout << "f3 = " << f3 << std::endl;
    BOOST_CHECK(f3.is_set(Test::Bla));

    f2 &= f3;
    std::cout << "f2 = " << f2 << " (after &= f3)" << std::endl;
    BOOST_CHECK(!f2.is_set(Test::Foo) and !f2.is_set(Test::Bla));
}

BOOST_AUTO_TEST_CASE(operator_xor_value)
{
    field f({ Test::Foo, Test::Bla });
    field g({ Test::Bla });
    std::cout << "-- Testing operator ^= with T" << std::endl
              << "f = " << f << " (before &=)" << std::endl
              << "g = " << g << " (expected value)" << std::endl;
    f ^= Test::Foo;
    BOOST_CHECK(f == g);
}

BOOST_AUTO_TEST_CASE(operator_xor_bitfield)
{
    field f({ Test::Foo, Test::Bla });
    std::cout << "-- Testing operator ^= with bitfield<T>" << std::endl << "f = " << f << " (before &=)" << std::endl;

    field g({ Test::Foo, Test::Bar });
    field h({ Test::Bar, Test::Bla });
    std::cout << "g = " << g << std::endl << "h = " << h << " (expected result)" << std::endl;

    f ^= g;
    std::cout << "f = " << f << " (after &= g)" << std::endl;
    BOOST_CHECK(f == h);
}

BOOST_AUTO_TEST_CASE(operator_negate)
{
    field f1(Test::Bla);
    std::cout << "-- Testing operator!" << std::endl << "f1 = " << f1 << std::endl;
    field f2 = !f1;
    field::type expected_value{0xfffb};
    std::cout << "Expected value is " << std::hex << expected_value << std::endl
              << "f2 = " << f2 << std::endl;
    BOOST_CHECK(f2.raw_value() == expected_value);
}
