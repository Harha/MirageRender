#ifndef MACROS_H
#define MACROS_H

#include <cstring>

// Fix __FILE__ to use relative path instead
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// Logging
#define LOG(msg)  std::cout << "LOG | " << msg << std::endl;
#define ERR(msg)  std::cerr << "ERR | " << msg << std::endl;

// Other stuff
#define DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;

#endif // MACROS_H
