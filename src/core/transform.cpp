// std includes
#include <iostream>

// mirage includes
#include "transform.h"
#include "../macros.h"

namespace mirage
{

Transform::Transform(vec4 position, quaternion orientation, vec4 scale) : m_position(position), m_orientation(orientation), m_scale(scale)
{

}

void Transform::setPosition(const vec4 &position)
{
    m_position = position;
}

void Transform::setOrientation(const quaternion &orientation)
{
    m_orientation = orientation;
}

void Transform::setScale(const vec4 &scale)
{
    m_scale = scale;
}

vec4 Transform::getPosition() const
{
    return m_position;
}

quaternion Transform::getOrientation() const
{
    return m_orientation;
}

vec4 Transform::getScale() const
{
    return m_scale;
}

mat4 Transform::getModelMatrix() const
{
    mat4 t;
    mat4 r;
    mat4 s;
    t.translation(m_position);
    r.rotation(m_orientation.getRightVector(), m_orientation.getUpVector(), m_orientation.getForwardVector());
    s.scale(m_scale);
    return t * r * s;
}

}
