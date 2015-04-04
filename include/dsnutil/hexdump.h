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

void hexdump(const void* mem, size_t length, std::ostream& out);

#ifdef WITH_LOG4CPP
#include <log4cpp/CategoryStream.hh>
void hexdump(const void* mem, size_t length, log4cpp::CategoryStream out);
#endif // WITH_LOG4CPP

#endif // !HEXDUMP_HH
