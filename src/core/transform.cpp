// std includes
#include <iostream>

// mirage includes
#include "transform.h"
#include "../macros.h"

namespace mirage
{

Transform::Transform(vec3 position, quaternion orientation, vec3 scale) : m_position(position), m_orientation(orientation), m_scale(scale)
{
    m_stateChanged = true;
}

void Transform::setPosition(const vec3 &position)
{
    if (m_position != position)
    {
        m_position = position;
        setState(true);
    }
}

void Transform::setOrientation(const quaternion &orientation)
{
    if(m_orientation != orientation)
    {
        m_orientation = orientation;
        setState(true);
    }
}

void Transform::setScale(const vec3 &scale)
{
    if (m_scale != scale)
    {
        m_scale = scale;
        setState(true);
    }
}

void Transform::setState(const bool state)
{
    m_stateChanged = state;
}

vec3 Transform::getPosition() const
{
    return m_position;
}

quaternion Transform::getOrientation() const
{
    return m_orientation;
}

vec3 Transform::getScale() const
{
    return m_scale;
}

Transform Transform::inverse() const
{
    Transform result;

    result.setPosition(m_position.negate());
    result.setOrientation(m_orientation.conjugate());

    return result;
}

mat4 Transform::getMatrix() const
{
    mat4 t;
    mat4 r;
    mat4 s;
    t.translation(m_position);
    r.rotation(m_orientation.getRightVector(), m_orientation.getUpVector(), m_orientation.getForwardVector());
    s.scale(m_scale);
    return t * r * s;
}

bool Transform::reqStateUpdate() const
{
    return m_stateChanged;
}

}
