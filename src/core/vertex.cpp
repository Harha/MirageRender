// std includes

// mirage includes
#include "vertex.h"

namespace mirage
{

	Vertex::Vertex(vec3 p, vec3 n, vec2 t) : m_p(p), m_n(n), m_t(t)
	{

	}

	void Vertex::setPosition(const vec3 &v)
	{
		m_p = v;
	}

	void Vertex::setNormal(const vec3 &n)
	{
		m_n = n.normalize();
	}

	void Vertex::setTexcoord(const vec2 &t)
	{
		m_t = t;
	}

	vec3 Vertex::getPosition() const
	{
		return m_p;
	}

	vec3 Vertex::getNormal() const
	{
		return m_n;
	}

	vec2 Vertex::getTexcoord() const
	{
		return m_t;
	}

}
