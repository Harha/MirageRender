// std includes
#include <iostream>

// mirage includes
#include "dielectric.h"
#include "../macros.h"

namespace mirage
{

	DielectricMaterial::DielectricMaterial(vec3 kd, vec3 ks, vec3 ke, float ior) : Material(nullptr, nullptr, nullptr, kd, ks, ke, true), m_ior(ior)
	{

	}

	DielectricMaterial::~DielectricMaterial()
	{

	}

	// Explaining fresnel equations and the schlick approximations:
	// https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
	// Optimized versions of the functions:
	// http://www.kevinbeason.com/smallpt/
	void DielectricMaterial::evalBSDF(const vec3 & P, const vec3 & N, const vec3 & Wr, const vec3 & Wt, const vec3 & Wo, float & brdf, float & btdf) const
	{
		// Are we going into the medium or out of it?
		auto normal = vec3::dot(N, Wo) > 0.0f ? N : N.negate();
		bool direct = vec3::dot(N, normal) > 0.0f;					// > 0.0 = going into the medium

		// Calculate the ratio of indices of refraction
		float eta = direct ? 1.0f / m_ior : m_ior;

		// Calculate variables for transmission or reflection
		float NdotWo = vec3::dot(normal, Wo);
		float cos2t = 1.0f - eta * eta * (1.0f - NdotWo * NdotWo);

		// Reflect the ray
		if (cos2t < 0.0f)
		{
			brdf = 1.0f;
			btdf = 0.0f;
			return;
		}

		// Calculate fresnel equations using Schlick's approximations
		// θi = incidence, θr = reflection, θt = refraction
		float n1 = m_ior - 1.0f;
		float n2 = m_ior + 1.0f;
		float R0 = n1 * n1 / (n2 * n2);								// R0 = ((n1 - n2) / (n1 + n2))^2
		float c = 1.0f - (direct ? NdotWo : vec3::dot(Wt, N));		// c  = (1.0 - cos(θi ? θt))^5
		float Rs = R0 + (1.0f - R0) * c * c * c * c * c;			// Rschlick(θi) = R0 + (1.0 - R0) * (1.0 - cos(θi ? θt))^5
		float Tr = 1.0f - Rs;
		float P_ = 0.5f * Rs + 0.25f;
		float R = Rs / P_;
		float T = Tr / (1.0f - P_);

		// Assign reflectivity and refractivity
		if (btdf > 2.0f)
		{
			float r = pseudorand();
			brdf = (r < P_) ? R : 0.0f;
			btdf = (r > P_) ? T : 0.0f;
		}
		else
		{
			brdf = Rs;
			btdf = Tr;
		}
	}

	void DielectricMaterial::evalBSDF_direct(const vec3 & P, const vec3 & N, const vec3 & We, const vec3 & Wr, const vec3 & Wt, const vec3 & Wo, float & brdf, float & btdf) const
	{
		brdf = 0.0f;
		btdf = 0.0f;
	}

	void DielectricMaterial::evalPDF(float & pdf) const
	{
		pdf = 1.0f;
	}

	void DielectricMaterial::evalWi(const vec3 & Wo, const vec3 & N, vec3 & Wr, vec3 & Wt)
	{
		// Are we going into the medium or out of it?
		auto normal = vec3::dot(N, Wo) > 0.0f ? N : N.negate();
		bool direct = vec3::dot(N, normal) > 0.0f;

		// Calculate the reflected ray
		Wr = vec3::reflect(Wo.negate(), N).normalize();

		// Calculate the ratio of indices of refraction
		float eta = direct ? 1.0f / m_ior : m_ior;

		// Refract the ray through the surface, Wt becomes |0.0f| if total internal reflection
		Wt = vec3::refract(Wo.negate(), normal, eta).normalize();
	}

}
