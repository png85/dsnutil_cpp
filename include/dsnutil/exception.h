#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

#include <dsnutil/dsnutil_cpp_Export.h>
#include <dsnutil/compiler_features.h>

namespace dsn {

class dsnutil_cpp_EXPORT Exception : public std::exception {
private:
    std::string m_description;
    std::string m_source;
    std::string m_file;
    size_t m_line;
    mutable std::string m_fullDescription;

public:
    Exception(const std::string& description, const std::string& source, const char* file, size_t line);
    Exception(const std::string& description, const std::string& source, const char* file, const char* line);

    ~Exception() throw();

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
