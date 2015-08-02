#ifndef TRANSFORM_H
#define TRANSFORM_H

// mirage includes
#include "../math/vec3.h"
#include "../math/quaternion.h"
#include "../math/mat4.h"

namespace mirage
{

class Transform
{
public:
    Transform(vec3 position = vec3(), quaternion orientation = quaternion(), vec3 scale = vec3(1, 1, 1));
    void setPosition(const vec3 &position);
    void setOrientation(const quaternion &orientation);
    void setScale(const vec3 &scale);
    void setState(const bool state);
    vec3 getPosition() const;
    quaternion getOrientation() const;
    vec3 getScale() const;
    Transform inverse() const;
    mat4 getMatrix() const;
    bool reqStateUpdate() const;
private:
    vec3 m_position;
    quaternion m_orientation;
    vec3 m_scale;
    bool m_stateChanged;
};

}

#endif // TRANSFORM_H
