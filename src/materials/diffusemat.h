#ifndef DIFFUSE_H
#define DIFFUSE_H

// std includes

// mirage includes
#include "../core/material.h"

namespace mirage
{

	class DiffuseMaterial : public virtual Material
	{
	public:
		DiffuseMaterial(Texture * const kdText = nullptr, vec3 kd = vec3(), vec3 ke = vec3());
		DiffuseMaterial(const std::string & kdText = "", vec3 kd = vec3(), vec3 ke = vec3());
		virtual ~DiffuseMaterial() override;
		virtual void evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const override;
		virtual void evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const override;
		virtual void evalPDF(float &pdf) const override;
		virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) override;
	private:
	};

}

#endif // DIFFUSE_H
