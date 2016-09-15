#ifndef MATERIAL_H
#define MATERIAL_H

// std includes

// mirage includes
#include "../math/vec3.h"
#include "ray.h"

namespace mirage
{

	struct MaterialInfo
	{
		MaterialInfo() { }

		vec3 kd;
		vec3 ks;
		vec3 ke;
		float r;
		float k;
		float d;
		float ior;
		int illum;
	};

	class Material
	{
	public:
		Material(vec3 kd = vec3(), vec3 ks = vec3(), vec3 ke = vec3(), bool refr = false);
		virtual ~Material();
		virtual void evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const = 0;
		virtual void evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const = 0;
		virtual void evalPDF(float &pdf) const = 0;
		virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) = 0;
		void setKd(const vec3 &v);
		void setKs(const vec3 &v);
		void setKe(const vec3 &v);
		vec3 getKd() const;
		vec3 getKs() const;
		vec3 getKe() const;
		bool isRefractive() const;
	protected:
		vec3 m_kd; // diffuse
		vec3 m_ks; // specular
		vec3 m_ke; // emittance
		bool m_refr;
	};

}

#endif // MATERIAL_H
