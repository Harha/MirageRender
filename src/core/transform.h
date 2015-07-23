#ifndef TRANSFORM_H
#define TRANSFORM_H

// mirage includes
#include "../math/vec4.h"
#include "../math/quaternion.h"
#include "../math/mat4.h"

namespace mirage
{

class Transform
{
public:
    Transform(vec4 position = vec4(), quaternion orientation = quaternion(), vec4 scale = vec4(1, 1, 1, 1));

    void setPosition(const vec4 &position)
    {
        m_position = position;
    }

    void setOrientation(const quaternion &orientation)
    {
        m_orientation = orientation;
    }

    void setScale(const vec4 &scale)
    {
        m_scale = scale;
    }

    vec4 getPosition() const
    {
        return m_position;
    }

    quaternion getOrientation() const
    {
        return m_orientation;
    }

    vec4 getScale() const
    {
        return m_scale;
    }

    mat4 getModelMatrix() const
    {
        mat4 t;
        mat4 r;
        mat4 s;
        t.translation(m_position);
        r.rotation(m_orientation.getRightVector(), m_orientation.getUpVector(), m_orientation.getForwardVector());
        s.scale(m_scale);
        return t * r * s;
    }

private:
    vec4 m_position;
    quaternion m_orientation;
    vec4 m_scale;
};

}

#endif // TRANSFORM_H
