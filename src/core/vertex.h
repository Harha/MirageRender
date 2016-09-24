#ifndef VERTEX_H
#define VERTEX_H

// std includes

// mirage includes
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat4.h"

namespace mirage
{

	class Vertex
	{
	public:
		Vertex(vec3 p = vec3(), vec3 n = vec3(), vec2 t = vec2());

		Vertex operator*(const mat4 &m) const
		{
			Vertex result;
			vec4 p_4;
			vec4 n_4;
			p_4 = m * vec4(m_p.x, m_p.y, m_p.z, 1.0f);
			n_4 = m * vec4(m_n.x, m_n.y, m_n.z, 0.0f);
			result.m_p = vec3(p_4.x, p_4.y, p_4.z);
			result.m_n = vec3(n_4.x, n_4.y, n_4.z).normalize();
			result.m_t = m_t;
			return result;
		}

		void setPosition(const vec3 &v);
		void setNormal(const vec3 &n);
		void setTexcoord(const vec2 &t);
		vec3 getPosition() const;
		vec3 getNormal() const;
		vec2 getTexcoord() const;
	private:
		vec3 m_p;
		vec3 m_n;
		vec2 m_t;
	protected:

	};

}

#endif // VERTEX_H
