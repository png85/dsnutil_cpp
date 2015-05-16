/** \file
 *
 * \brief Throwing implementation of assert(3)
 *
 * This file redefines the assert() macro known from C so that it throws
 * a std::logic_error exception containing the filename, line number and
 * description of the assertion that failed.
 */
#ifndef DSNUTIL_ASSERT_HPP
#define DSNUTIL_ASSERT_HPP 1

#include <dsnutil/exception.h>
#include <stdexcept>
#include <string>
#include <cassert>

#ifndef NDEBUG

// disable existing assert macro
#undef assert

/** \brief Redefined assert(3)
 */
#define assert(test) assert2(test, __FILE__, __LINE__)
/** \brief Helper macro to redefine assert(3)
 */
#define assert2(test, file, line) assert3(test, file, line)

/** \brief Helper function for throwing assert(3)
 *
 * This is used to redefine the assert() macro from C with an implementation
 * that throws an exception.
 *
 * \param test Test condition for the assertion
 * \param test_str Test condition as string
 * \param source Source function name
 * \param file Source filename
 * \param line Source line
 *
 * \throw dsn::Exception with a message about the failed assertion if test is false.
 */
inline void throwing_assert(bool test, const char* test_str, const char* source, const char* file, size_t line)
{
    if (!test) {
        throw dsn::Exception(std::string("Assertion '").append(test_str).append("' failed!"), source, file, line);
    }
}

#if (__GNUC__ == 2 && __GNUC_MINOR__ >= 6) || __GNUC__ > 2 || defined(__clang__)
/** \brief Throwing assert(3) definition
 */
#define assert3(test, file, line) \
    (::throwing_assert(test, #test, __PRETTY_FUNCTION__, file, line));
#else
/** \brief Throwing assert(3) definition
 */
#define assert3(test, file, line) \
    (::throwing_assert(test, #test, #file ":" #line, file, line));
#endif

#endif // !NDEBUG

#endif // !DSNUTIL_ASSERT_HPP
