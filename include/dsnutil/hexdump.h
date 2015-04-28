/// $Id$
///
/// (c) 2011 DAS-SYSTEM Networks
///
/// \file
/// \brief Prototype for hexdump()
///
/// \author Peter Hille <peter@das-system-networks.de>
///
#ifndef HEXDUMP_HH
#define HEXDUMP_HH 1

#include <iostream>

#include <dsnutil/dsnutil_cpp_Export.h>

void dsnutil_cpp_EXPORT hexdump(const void* mem, size_t length, std::ostream& out);

#ifdef WITH_LOG4CPP
#include <log4cpp/CategoryStream.hh>
void dsnutil_cpp_EXPORT hexdump(const void* mem, size_t length, log4cpp::CategoryStream out);
#endif // WITH_LOG4CPP

#endif // !HEXDUMP_HH
