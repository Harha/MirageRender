#ifndef RAY_H
#define RAY_H

// mirage includes
#include "../math/vec4.h"

namespace mirage
{

class Ray
{
public:
    Ray(vec4 origin = vec4(), vec4 direction = vec4());

    void setOrigin(const vec4 &origin);
    void setDirection(const vec4 &direction);
    vec4 getOrigin() const;
    vec4 getDirection() const;
    vec4 getDirectionInv() const;
private:
    vec4 m_origin;
    vec4 m_direction;
    vec4 m_directionInv;
};

}

#endif // RAY_H
