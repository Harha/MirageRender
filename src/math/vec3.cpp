// mirage includes
#include "vec3.h"

namespace mirage
{

std::string vec3::toString() const
{
    char buffer[255];
    snprintf(buffer, 255, "vec3[%+.2f %+.2f %+.2f]", x, y, z);
    return std::string(buffer);
}

}
