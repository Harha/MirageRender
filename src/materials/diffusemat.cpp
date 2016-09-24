// std includes
#include <iostream>

// mirage includes
#include "diffusemat.h"
#include "../macros.h"

namespace mirage
{

	DiffuseMaterial::DiffuseMaterial(Texture * const kdText, vec3 kd, vec3 ke) :
		Material(kdText, nullptr, nullptr, kd, 0.0f, ke)
	{

	}

	DiffuseMaterial::DiffuseMaterial(const std::string & kdText, vec3 kd, vec3 ke) :
		Material(kdText, "", "", kd, 0.0f, ke)
	{

	}

	DiffuseMaterial::~DiffuseMaterial()
	{

	}

	void DiffuseMaterial::evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
	{
		// Calculate the cosi term
		float cosi = vec3::dot(Wr, N);

		// Calculate the lambertian brdf
		brdf = PI_INV * cosi;

		// There's no transmission, so btdf stays 0.0f
		btdf = 0.0f;
	}

	void DiffuseMaterial::evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
	{
		// Calculate the cosi term
		float cosi = std::max(vec3::dot(We, N), 0.0f);

		// Calculate the lambertian brdf
		brdf = PI_INV * cosi;

		// There's no transmission, so btdf stays 0.0f
		btdf = 0.0f;
	}

	void DiffuseMaterial::evalPDF(float &pdf) const
	{
		pdf = 1.0f / (2.0f * PI);
	}

	void DiffuseMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
	{
		// Uniform hemispherical sampling
		Wr = vec3::sampleHemisphere(N).normalize();

		// No transmission, Wt stays 0.0f
		Wt = vec3();
	}

}
