#ifndef RAY_H
#define RAY_H

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

	class Ray
	{
	public:
		Ray(vec3 origin = vec3(), vec3 direction = vec3(), float tmin = 0.0f, float tmax = INFINITY);
		void setOrigin(const vec3 &origin);
		void setDirection(const vec3 &direction);
		vec3 getOrigin() const;
		vec3 getDirection() const;
		vec3 getDirectionInv() const;
		vec3 operator()(float t) const;
		float mint, maxt;
	private:
		vec3 m_origin;
		vec3 m_direction;
		vec3 m_directionInv;
	};

}

#endif // RAY_H
