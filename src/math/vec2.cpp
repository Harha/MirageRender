#include "vec2.h"

namespace mirage
{

	std::string vec2::toString() const
	{
		char buffer[255];
		snprintf(buffer, 255, "vec2[%+.2f %+.2f]", x, y);
		return std::string(buffer);
	}

}
