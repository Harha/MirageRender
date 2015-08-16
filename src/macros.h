#ifndef MACROS_H
#define MACROS_H

#include <cstring>

// Fix __FILE__ to use relative path instead
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Logging
#define LOG(msg)  std::cout << "LOG | " << msg << std::endl;
#define ERR(msg)  std::cerr << "ERR | " << msg << std::endl;
//#define LOG(msg)  std::cout << __FILENAME__ << "(" << __LINE__ << "): " << msg << std::endl;
//#define ERR(msg)  std::cerr << "ERR: " << __FILENAME__ << "(" << __LINE__ << "): " << msg << std::endl;

#endif // MACROS_H
