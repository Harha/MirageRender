// std includes
#include <iostream>

// mirage includes
#include "accelerator.h"
#include "../macros.h"

namespace mirage
{

	Accelerator::Accelerator(const std::vector<Shape *> shapes) :
		m_shapes(shapes),
		m_initialized(false)
	{
	}

	Accelerator::~Accelerator()
	{

	}

	AABB Accelerator::objectBound() const
	{
		AABB result = m_shapes[0]->objectBound();

		for (size_t i = 0; i < m_shapes.size(); i++)
		{
			result = result.addBox(m_shapes[i]->objectBound());
		}

		return result;
	}

	AABB Accelerator::worldBound() const
	{
		AABB result = m_shapes[0]->worldBound();

		for (size_t i = 0; i < m_shapes.size(); i++)
		{
			result = result.addBox(m_shapes[i]->worldBound());
		}

		return result;
	}

}
