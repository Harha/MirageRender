// mirage includes
#include "vec4.h"

namespace mirage
{

std::string vec4::toString() const
{
    char buffer[255];
    snprintf(buffer, 255, "vec4[%+.2f %+.2f %+.2f %+.2f]", x, y, z, w);
    return std::string(buffer);
}

}
