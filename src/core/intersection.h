#ifndef INTERSECTION_H
#define INTERSECTION_H

// mirage includes
#include "../math/vec3.h"
#include "../math/vec2.h"
#include "material.h"

namespace mirage
{

	class Intersection
	{
	public:
		Intersection(float t = INFINITY, vec3 position = vec3(), vec3 normal = vec3(), vec2 texcoord = vec2(), Material * material = nullptr);
		void setT(const float t);
		void setPosition(const vec3 & v);
		void setNormal(const vec3 & v);
		void setTexcoord(const vec2 & v);
		void setMaterial(Material * m);
		float getT() const;
		vec3 getPosition() const;
		vec3 getNormal() const;
		vec2 getTexcoord() const;
		Material *getMaterial() const;
	private:
		float m_t;
		vec3 m_position;
		vec3 m_normal;
		vec2 m_texcoord;
		Material *m_material;
	};

}

#endif // INTERSECTION_H
