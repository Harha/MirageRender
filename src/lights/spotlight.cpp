// std includes
#include <iostream>

// mirage includes
#include "spotlight.h"
#include "../macros.h"

namespace mirage
{

	SpotLight::SpotLight(Transform l2w, vec3 emission, float aC, float aL, float aQ, float cutoff) : Light(l2w, emission), m_attenuationC(aC), m_attenuationL(aL), m_attenuationQ(aQ), m_cutoff(cutoff)
	{
		LOG("SpotLight: Created a new spot light source.");
	}

	void SpotLight::Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Le) const
	{
		// Calculate amount of emitted light
		float spot_factor = vec3::dot(Wi.normalize(), m_lightToWorld.getOrientation().getForwardVector().negate());

		// Set emittance to 0 by default
		Le = vec3(0, 0, 0);

		// If the world surface position is inside the area to be shaded, calculate emittance
		if (spot_factor > m_cutoff)
		{
			// Calculate distance from light to P
			float d = Wi.length();

			// Get light attenuatin factor
			float attenuation = 1.0f / (m_attenuationC + m_attenuationL * d + m_attenuationQ * d * d);

			// Get incident radiation
			Le = m_emission * attenuation;
			Le *= (1.0f - (1.0f - spot_factor) / (1.0f - m_cutoff));
		}
	}

	void SpotLight::evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const
	{
		We = m_lightToWorld.getPosition() - P;
	}

}
