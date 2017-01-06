#ifndef GLASS_H
#define GLASS_H

// std includes

// mirage includes
#include "../core/material.h"

namespace mirage
{

	class DielectricMaterial : public virtual Material
	{
	public:
		DielectricMaterial(vec3 kd = vec3(0.9f, 0.9f, 0.9f), vec3 ks = vec3(), vec3 ke = vec3(), float ior = 1.52f);
		virtual ~DielectricMaterial() override;
		virtual void evalBSDF(const vec3 & P, const vec3 & N, const vec3 & Wr, const vec3 & Wt, const vec3 & Wo, float & brdf, float & btdf) const override;
		virtual void evalBSDF_direct(const vec3 & P, const vec3 & N, const vec3 & We, const vec3 & Wr, const vec3 & Wt, const vec3 & Wo, float & brdf, float & btdf) const override;
		virtual void evalPDF(float & pdf) const override;
		virtual void evalWi(const vec3 & Wo, const vec3 & N, vec3 & Wr, vec3 & Wt) override;
	private:
		float m_ior;
	};

}

#endif // GLASS_H
