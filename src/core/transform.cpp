// std includes
#include <iostream>

// mirage includes
#include "transform.h"
#include "../macros.h"

namespace mirage
{

Transform::Transform(vec4 position, quaternion orientation, vec4 scale) : m_position(position), m_orientation(orientation), m_scale(scale) { }

}
