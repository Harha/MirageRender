// std includes
#include <iostream>

// mirage includes
#include "specmat.h"
#include "../macros.h"

namespace mirage
{

	SpecularMaterial::SpecularMaterial(vec3 kd, vec3 ks, vec3 ke) : Material(nullptr, nullptr, nullptr, kd, ks, ke)
	{

	}

	SpecularMaterial::~SpecularMaterial()
	{

	}

	void SpecularMaterial::evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
	{
		brdf = 1.0f;
		btdf = 0.0f;
	}

	void SpecularMaterial::evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
	{
		// Check if the supplied Wr really is the same
		if (We == Wr)
		{
			brdf = 1.0f;
		}
		else
		{
			brdf = 0.0f;
		}

		// There's no transmission, so btdf stays 0.0f
		btdf = 0.0f;
	}

	void SpecularMaterial::evalPDF(float &pdf) const
	{
		pdf = 1.0f;
	}

	void SpecularMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
	{
		Wr = vec3::reflect(Wo.negate(), N).normalize();
		Wt = vec3();
	}

}
