#ifndef AABB_H
#define AABB_H

// std includes
#include <string>

// mirage includes
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat4.h"
#include "ray.h"

namespace mirage
{

	class AABB
	{
	public:
		AABB(vec3 p1 = vec3(-1, -1, -1), vec3 p2 = vec3(1, 1, 1));

		std::string toString() const;

		AABB operator*(const mat4 &m) const
		{
			AABB result;
			vec4 pmin(m_pmin.x, m_pmin.y, m_pmin.z, 1.0f);
			vec4 pmax(m_pmax.x, m_pmax.y, m_pmax.z, 1.0f);
			pmin = m * pmin;
			pmax = m * pmax;
			result.m_pmin = vec3(pmin.x, pmin.y, pmin.z);
			result.m_pmax = vec3(pmax.x, pmax.y, pmax.z);
			return result;
		}

		AABB addPoint(const vec3 &p) const;
		AABB addBox(const AABB &b) const;
		AABB expand(const float delta) const;
		bool intersectP(const Ray &ray, float &tHit1, float &tHit2) const;
		bool overlaps(const AABB &b) const;
		bool inside(const vec3 &p) const;
		vec3 getCentroid() const;
		float getSurfaceArea() const;
		float getVolume() const;
		int getMaximumExtent() const;
	private:
		vec3 m_pmin;
		vec3 m_pmax;
	};

}

#endif // AABB_H
