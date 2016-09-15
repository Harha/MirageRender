// std includes

// mirage includes
#include "light.h"

namespace mirage
{

	Light::Light(Transform l2w, vec3 emission) : m_lightToWorld(l2w), m_emission(emission)
	{

	}

	Light::~Light()
	{

	}

	Transform Light::getL2W() const
	{
		return m_lightToWorld;
	}

}
