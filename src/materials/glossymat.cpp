// std includes
#include <iostream>

// mirage includes
#include "glossymat.h"
#include "../macros.h"

namespace mirage
{

	GlossyMaterial::GlossyMaterial(vec3 kd, vec3 ks, vec3 ke, float r, float k, float d) : Material(nullptr, nullptr, nullptr, kd, ks, ke), m_r(r), m_k(k), m_d(d)
	{

	}

	GlossyMaterial::~GlossyMaterial()
	{

	}

	// The cook-torrance microfacet model:
	// http://ruh.li/GraphicsCookTorrance.html
	void GlossyMaterial::evalBSDF(const vec3 & P, const vec3 & N, const vec3 & Wr, const vec3 & Wt, const vec3 & Wo, float & brdf, float & btdf) const
	{
		// Surface properties
		float R = m_r + EPSILON; // Roughness
		float RR = R * R;
		float F = m_k; // Fresnel reflectance
		float K = m_d; // Fraction of diffuse reflection

		// Calculate the half vector
		auto halfV = (Wo + Wr).normalize();

		// Dot products at the incident
		float NdotWo = vec3::dot(N, Wo); // θi = incidence
		float NdotWr = vec3::dot(N, Wr); // θr = reflection
		float NdotH = vec3::dot(N, halfV);
		float WodotH = vec3::dot(Wo, halfV);

		// Calculate geometric attenuation
		float g_numer = 2.0f * NdotH;
		float geometric = std::min(1.0f, std::min((g_numer * NdotWo) / WodotH, (g_numer * NdotWr) / WodotH));

		// Calculate the microfacet distribution based roughness
		float r_scalar = 1.0f / (4.0f * RR * NdotH * NdotH * NdotH * NdotH);
		float r_exponent = (NdotH * NdotH - 1.0f) / (RR * NdotH * NdotH);
		float roughness = r_scalar * std::exp(r_exponent);

		// Calculate fresnel equations using Schlick's approximations
		float fresnel = std::pow(1.0f - WodotH, 5.0f);
		fresnel *= (1.0f - F);
		fresnel += F;

		// Combine the terms to get reflectance
		float Rs = (geometric * roughness * fresnel) / (PI * NdotWo * NdotWr + EPSILON);

		// Get the final BRDF scalar value, transmission is 0.0
		brdf = PI_INV * NdotWr * (Rs * (1.0f - K) + K);
		btdf = 0.0f;
	}

	// The cook-torrance microfacet model:
	// http://ruh.li/GraphicsCookTorrance.html
	void GlossyMaterial::evalBSDF_direct(const vec3 & P, const vec3 & N, const vec3 & We, const vec3 & Wr, const vec3 & Wt, const vec3 & Wo, float & brdf, float & btdf) const
	{
		// Surface properties
		float R = m_r + EPSILON; // Roughness
		float RR = R * R;
		float F = m_k; // Fresnel reflectance
		float K = m_d; // Fraction of diffuse reflection

		// Calculate the half vector
		auto halfV = (Wo + We).normalize();

		// Dot products at the incident
		float NdotWo = vec3::dot(N, Wo); // θi = incidence
		float NdotWe = vec3::dot(N, We); // θr = reflection
		float NdotH = vec3::dot(N, halfV);
		float WodotH = vec3::dot(Wo, halfV);

		// Calculate geometric attenuation
		float g_numer = 2.0f * NdotH;
		float geometric = std::min(1.0f, std::min((g_numer * NdotWo) / WodotH, (g_numer * NdotWe) / WodotH));

		// Calculate the microfacet distribution based roughness
		float r_scalar = 1.0f / (4.0f * RR * NdotH * NdotH * NdotH * NdotH);
		float r_exponent = (NdotH * NdotH - 1.0f) / (RR * NdotH * NdotH);
		float roughness = r_scalar * std::exp(r_exponent);

		// Calculate fresnel equations using Schlick's approximations
		float fresnel = std::pow(1.0f - WodotH, 5.0f);
		fresnel *= (1.0f - F);
		fresnel += F;

		// Combine the terms to get reflectance
		float Rs = (geometric * roughness * fresnel) / (PI * NdotWo * NdotWe + EPSILON);

		// Get the final BRDF scalar value, transmission is 0.0
		brdf = PI_INV * NdotWe * (Rs * (1.0f - K) + K);
		btdf = 0.0f;
	}

	void GlossyMaterial::evalPDF(float & pdf) const
	{
		pdf = 1.0f / (2.0f * PI * m_r);
	}

	void GlossyMaterial::evalWi(const vec3 & Wo, const vec3 & N, vec3 & Wr, vec3 & Wt)
	{
		// Calculate mirror & random reflection vectors
		vec3 Wr_mirr = vec3::reflect(Wo.negate(), N).normalize();
		vec3 Wr_rand = vec3::sampleHemisphere(Wr_mirr, m_r, 1.0f - m_r);

		// Make sure the random ray doesn't go through the hit surface
		if (vec3::dot(N, Wr_rand) < 0.0f)
			Wr_rand = vec3::reflect(Wr_rand, N);

		// Assign the reflected ray to Wr
		Wr = Wr_rand.normalize();

		// No transmission, so Wt stays 0.0f
		Wt = vec3();
	}

}
