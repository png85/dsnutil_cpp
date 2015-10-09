#include <dsnutil/exception.h>
#include <sstream>
#include <cstdlib>

using namespace dsn;

Exception::Exception(const std::string& description, const std::string& source, const char* file, size_t line)
    : m_description(description)
    , m_source(source)
    , m_file(file)
    , m_fullDescription("")
    , m_line(line)
{
}

Exception::Exception(const std::string& description, const std::string& source, const char* file, const char* line)
    : m_description(description)
    , m_source(source)
    , m_file(file)
    , m_fullDescription("")
    , m_line(atoi(line))
{
}

Exception::~Exception() dsnutil_cpp_NOEXCEPT
{
}

/// \brief Return full human-readable error message
///
/// \return Human-readable error description including source, file and line number
const std::string& Exception::fullDescription() const
{
    if (m_fullDescription.empty()) {
        std::stringstream ss;
        ss << "(" << m_file << ":" << m_line << "): Exception at " << m_source << ": " << m_description;
        m_fullDescription = ss.str();
    }

    return m_fullDescription;
}

/// \brief Get exception source
///
/// For Clang, GCC and MSVC this returns the actual function signature, on other compilers it'll default
/// to the source code filename.
///
/// \return Exception source as human-readable string
const std::string& Exception::getSource() const
{
    return m_source;
}

/// \brief Get source file
///
/// \return Name of the source file that triggered the exception
const std::string& Exception::getFile() const
{
    return m_file;
}

/// \brief Get error description
///
/// \return Human-readable error description
const std::string& Exception::getDescription() const
{
    return m_description;
}

/// \brief Get full error description
///
/// \see getFullDescription
///
/// \return Full human-readable error description
const char* Exception::what() const dsnutil_cpp_NOEXCEPT
{
    return fullDescription().c_str();
}
