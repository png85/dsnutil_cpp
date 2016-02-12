// -*- C++ -*-
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

#include <dsnutil/compiler_features.h>
#include <dsnutil/dsnutil_cpp_Export.h>

namespace dsn {

/// \brief Base class for exceptions in dsn code
///
/// This class is the base for all exceptions thrown by code from this library
class dsnutil_cpp_EXPORT Exception : public std::exception {
private:
    /// \brief Human-readable error description
    std::string m_description;

    /// \brief Function/method file that triggered the exception
    std::string m_source;

    /// \brief Source file that triggered the exception
    std::string m_file;

    /// \brief Line number where the exception was triggered
    size_t m_line;

    /// \brief Full human-readable error text
    ///
    /// This is initialized from the values of the other class members and kept for
    /// performance reasons.
    mutable std::string m_fullDescription;

public:
    Exception(const std::string& description, const std::string& source, const char* file, size_t line);
    Exception(const std::string& description, const std::string& source, const char* file, const char* line);

    ~Exception() dsnutil_cpp_NOEXCEPT;

    Exception(const Exception&) = default;
    Exception& operator=(const Exception&) = default;

    const std::string& fullDescription() const;
    const std::string& getSource() const;
    const std::string& getFile() const;
    const std::string& getDescription() const;

    // exception interface
public:
    const char* what() const dsnutil_cpp_NOEXCEPT;
};
}

#if (dsnutuil_cpp_COMPILER_IS_GNU || dsnutil_cpp_COMPILER_IS_Clang)
// function signature for GCC/Clang
#define DSN_EXCEPTION_SOURCE __PRETTY_FUNCTION__
#elif (dsnutil_cpp_COMPILER_IS_MSVC)
// function signature for MSVC
#define DSN_EXCEPTION_SOURCE __FUNCSIG__
#else
// other compilers default to the filename
#define DSN_EXCEPTION_SOURCE __FILE__
#endif

#define DSN_EXCEPTION(baseClass, descr, src) throw baseClass(descr, src, __FILE__, __LINE__)
#define DSN_EXCEPTION_SIMPLE(baseClass, descr) DSN_EXCEPTION(baseClass, descr, DSN_EXCEPTION_SOURCE)

#define DSN_DEFAULT_EXCEPTION(descr, src) DSN_EXCEPTION(dsn::Exception, descr, src)
#define DSN_DEFAULT_EXCEPTION_SIMPLE(descr) DSN_EXCEPTION_SIMPLE(dsn::Exception, descr)

#endif // EXCEPTION_H
