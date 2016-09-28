#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <cstring>

// Fix __FILE__ to use relative path instead
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Logging
#define LOG(msg)  std::cout << "LOG | " << msg << std::endl;
#define ERR(msg)  std::cerr << "ERR | " << msg << std::endl;

// Utility stuff
#define DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#define DELETEA(a) if ( (a) != NULL ) delete[] (a); (a) = NULL;

// Operating System Macros
#define OS_WINDOWS  (defined(_WIN64) || defined(_WIN32) ||  defined(__WIN32__) || defined(__TOS_WIN__) || defined(__CYGWIN__) || defined(__CYGWIN32) || defined(__MINGW32__) || defined(__BORLANDC__) || defined(__WINDOWS__))
#define OS_MACOS    (defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__))
#define OS_LINUX    (defined(__linux__) || defined(__linux))

#endif // MACROS_H
