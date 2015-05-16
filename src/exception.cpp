#include <dsnutil/exception.h>
#include <sstream>

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

Exception::~Exception()
{
}

const std::string& Exception::fullDescription() const
{
    if (m_fullDescription.empty()) {
        std::stringstream ss;
        ss << "(" << m_file << ":" << m_line << "): Exception at " << m_source << ": " << m_description;
        m_fullDescription = ss.str();
    }

    return m_fullDescription;
}

const std::string& Exception::getSource() const
{
    return m_source;
}

const std::string& Exception::getFile() const
{
    return m_file;
}

const std::string& Exception::getDescription() const
{
    return m_description;
}

const char* Exception::what() const noexcept
{
    return fullDescription().c_str();
}
