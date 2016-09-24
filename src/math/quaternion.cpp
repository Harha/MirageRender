// mirage includes
#include "quaternion.h"

namespace mirage
{

	std::string quaternion::toString() const
	{
		char buffer[255];
		snprintf(buffer, 255, "quat[%+.2f %+.2f %+.2f %+.2f]", w, x, y, z);
		return std::string(buffer);
	}

}
