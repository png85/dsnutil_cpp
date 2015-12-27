/** \file
 * \brief Implementation of hexdump()
 *
 * This file contains an implementation of hexdump() which can be used
 * to write a hex dump of arbitrary memory locations to ostreams and
 * log4cpp targets.
 *
 * \author Peter Hille (png!das-system) <peter@das-system-networks.de>
 *
 * \version 1.1 Moved all hexdump related code to hexdump.cc
 *
 * \version 1.0 Initial release
 */
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <dsnutil/hexdump.h>

#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>

#ifdef WITH_LOG4CPP
#include <log4cpp/CategoryStream.hh>
#endif

/** \brief Print hex dump of a memory area
 *
 * This function pretty-prints a hex-dump of the memory area pointed to by
 * \a mem to an ostream. The output is roughly similar to 'hexdump -C'
 * on UNIX systems.
 *
 * \param mem Pointer to the memory area to be dumped
 * \param length Number of bytes to dump
 * \param out Output stream to which the hexdump shall be written
 */
void hexdump(const void* mem, size_t length, std::ostream& out)
{
    char line[80];
    const char* src = static_cast<const char*>(mem);

    for (unsigned int i = 0; i < length; i += 16, src += 16) {
        char* t = line;
        // each line begins with the offset...
        t += sprintf(t, "%04x: ", i);

        // ... then we print the data in hex byte-wise...
        for (int j = 0; j < 16; j++) {
            if (i + j < length) {
                t += sprintf(t, "%02X", src[j] & 0xff);
            } else {
                t += sprintf(t, "  ");
            }

            // print a tabulator after the first group of 8 bytes
            if (j == 7) {
                t += sprintf(t, "\t");
            } else {
                t += sprintf(t, " ");
            }
        }

        // ... and finally we display all printable characters
        t += sprintf(t, "|");
        for (int j = 0; j < 16; j++) {
            if (i + j < length) {
                if (isprint(static_cast<unsigned char>(src[j])) != 0) {
                    t += sprintf(t, "%c", src[j]);
                } else {
                    t += sprintf(t, ".");
                }
            } else {
                t += sprintf(t, " ");
            }
        }

        t += sprintf(t, "|\n");
        out << line;
    }
}

#ifdef WITH_LOG4CPP
/** \brief Print hex dump of a memory area
 *
 * This function pretty-prints a hex-dump of the memory area pointed to by
 * \a mem to a log4cpp::CategoryStream.
 * The output is roughly similar to 'hexdump -C' on UNIX systems.
 *
 * \param mem Pointer to the memory area to be dumped
 * \param length Number of bytes to dump
 * \param out Output stream to which the hexdump shall be written
 */
void hexdump(const void* mem, size_t length, log4cpp::CategoryStream out)
{
    char line[80];
    const char* src = static_cast<const char*>(mem);

    out << "Hexdumping " << length << " bytes:\n";

    for (unsigned int i = 0; i < length; i += 16, src += 16) {
        char* t = line;
        // each line begins with the offset...
        t += sprintf(t, "%04x: ", i);

        // ... then we print the data in hex byte-wise...
        for (int j = 0; j < 16; j++) {
            if (i + j < length)
                t += sprintf(t, "%02X", src[j] & 0xff);
            else
                t += sprintf(t, "  ");

            // print a tabulator after the first group of 8 bytes
            if (j == 7)
                t += sprintf(t, "\t");
            else
                t += sprintf(t, " ");
        }

        // ... and finally we display all printable characters
        t += sprintf(t, "|");
        for (int j = 0; j < 16; j++) {
            if (i + j < length) {
                if (isprint((unsigned char)src[j]))
                    t += sprintf(t, "%c", src[j]);
                else
                    t += sprintf(t, ".");
            } else {
                t += sprintf(t, " ");
            }
        }

        t += sprintf(t, "|");
        out << line << "\n";
    }
}
#endif // WITH_LOG4CP
