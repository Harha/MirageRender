#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <cstring>

// Fix __FILE__ to use relative path instead
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Logging
#define LOG(msg)  std::cout << "LOG | " << __FILENAME__ << " | " << msg << std::endl;
#define WRN(msg)  std::cout << "WRN | " << __FILENAME__ << " | " << msg << std::endl;
#define ERR(msg)  std::cerr << "ERR | " << __FILENAME__ << " | " << msg << std::endl;

// Utility stuff
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
