#ifndef STRUTILS_H
#define STRUTILS_H

// std includes
#include <string>

// mirage includes

namespace mirage
{

namespace util
{

bool bothAreSpaces(char lhs, char rhs)
{
    return (lhs == rhs) && (lhs == ' ');
}

std::string removeDuplicateCharacters(const std::string str, const std::string c)
{
    return std::regex_replace(str, std::regex("['" + c + "']{2,}"), c);
}

}

}

#endif // STRUTILS_H
