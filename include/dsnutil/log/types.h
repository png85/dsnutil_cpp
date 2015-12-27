#ifndef DSNUTIL_LOG_TYPES_H
#define DSNUTIL_LOG_TYPES_H

#include <boost/log/trivial.hpp>
#include <dsnutil/compiler_features.h>

#ifdef dsnutil_cpp_COMPILER_CXX_ALIAS_TEMPLATES
using severity = boost::log::trivial::severity_level;
#else
typedef boost::log::trivial::severity_level severity;
#endif

#endif // DSNUTIL_LOG_TYPES_H
