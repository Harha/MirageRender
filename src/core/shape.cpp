// std includes
#include <iostream>

// mirage includes
#include "shape.h"
#include "../macros.h"
#include "../math/math.h"

namespace mirage
{

	Shape::Shape(const Transform o2w, Material *m) : m_objToWorld(o2w), m_worldToObj(o2w.inverse()), m_material(m)
	{

	}

	Shape::~Shape()
	{

	}

	AABB Shape::worldBound() const
	{
		return objectBound() * m_objToWorld.getMatrix();
	}

	void Shape::setMaterial(Material &m)
	{
		m_material = &m;
	}

	Material *Shape::getMaterial() const
	{
		return m_material;
	}

}
