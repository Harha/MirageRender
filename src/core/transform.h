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

    void setPosition(const vec4 &position);
    void setOrientation(const quaternion &orientation);
    void setScale(const vec4 &scale);
    vec4 getPosition() const;
    quaternion getOrientation() const;
    vec4 getScale() const;
    mat4 getModelMatrix() const;
private:
    vec4 m_position;
    quaternion m_orientation;
    vec4 m_scale;
};

}

#endif // TRANSFORM_H
