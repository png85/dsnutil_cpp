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

#include <stdexcept>
#include <string>
#include <cassert>

#ifndef NDEBUG

#  undef assert
/** \brief Redefined assert(3)
 */
#  define assert(test) assert2(test, __FILE__, __LINE__)
/** \brief Helper macro to redefine assert(3)
 */
#  define assert2(test, file, line) assert3(test, file, line)

/** \brief Helper function for throwing assert(3)
 *
 * This is used to redefine the assert() macro from C with an implementation
 * that throws an exception.
 *
 * \param test Test condition for the assertion
 * \param msg Message text of the exception that shall be thrown
 *
 * \throw std::logic_error with the given message text if the test condition
 * is false
 */
inline void throwing_assert(bool test, const std::string& msg) {
  if (!test)
    throw std::logic_error(msg);
}

#  if (__GNUC__ == 2 && __GNUC_MINOR__ >= 6) || __GNUC__ > 2 || \
  defined(__clang__)
/** \brief Throwing assert(3) definition
 */
#    define assert3(test, file, line)					\
  (::throwing_assert(test,						\
		     std::string(file ":" #line ": ")			\
		     .append(__PRETTY_FUNCTION__)			\
		     .append(": assertion '" #test "' failed.")))
#  else
/** \brief Throwing assert(3) definition
 */
#    define assert3(test, file, line) \
  (::throwing_assert(test,						\
		     file ":" #line ": assertion '" #test "' failed."))
#  endif

#endif // !NDEBUG

#endif // !DSNUTIL_ASSERT_HPP
