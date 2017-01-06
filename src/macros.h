#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <cstring>

// Software version
#define VERSION_R 0
#define VERSION_B 1
#define VERSION_A 2

// Gamma correction
#define GAMMA 1.0f / 2.2f

// Fix __FILE__ to use relative path instead
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Logging (deprecated)
#define LOG(msg)  std::cout << "LOG | " << msg << std::endl;
#define WRN(msg)  std::cout << "WRN | " << msg << std::endl;
#define ERR(msg)  std::cerr << "ERR | " << msg << std::endl;

// Logging
#define MLOG_LEVEL 3

#if MLOG_LEVEL > 3
#define MLOG_DEBUG(msg_, ...)	printf("DBG | " msg_ "\n", __VA_ARGS__)
#else
#define MLOG_DEBUG(msg, ...) do {} while(0)
#endif

#if MLOG_LEVEL > 2
#define MLOG_INFO(msg_, ...)	printf("INF | " msg_ "\n", __VA_ARGS__)
#else
#define MLOG_INFO(msg, ...) do {} while(0)
#endif

#if MLOG_LEVEL > 1
#define MLOG_WARNING(msg_, ...)	printf("WRN | " msg_ "\n", __VA_ARGS__)
#else
#define MLOG_WARNING(msg, ...) do {} while(0)
#endif

#if MLOG_LEVEL > 0
#define MLOG_ERROR(msg_, ...)	printf("ERR | " msg_ "\n", __VA_ARGS__)
#else
#define MLOG_ERROR(msg, ...) do {} while(0)
#endif

// Delete
#define DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#define DELETEA(a) if ( (a) != NULL ) delete[] (a); (a) = NULL;

// Operating system detection macros
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define OS_WINDOWS
#endif

#if defined(__linux__) || defined(__gnu_linux__)
#define OS_LINUX
#endif

#if defined(__APPLE__) || defined(__MACH__) || defined(macintosh)
#define OS_MACOS
#endif

#endif // MACROS_H
