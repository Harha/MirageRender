#ifndef MATERIAL_H
#define MATERIAL_H

// std includes
#include <string>

// mirage includes
#include "../math/vec3.h"
#include "ray.h"

namespace mirage
{

	// Forward declarations
	class Texture;

	class Material
	{
	public:
		Material(Texture * const kdText = nullptr, Texture * const ksText = nullptr, Texture * const keText = nullptr,
			vec3 kd = vec3(), vec3 ks = vec3(), vec3 ke = vec3(), bool refr = false);
		Material(const std::string & kdText = "", const std::string & ksText = "", const std::string & keText = "",
			vec3 kd = vec3(), vec3 ks = vec3(), vec3 ke = vec3(), bool refr = false);
		virtual ~Material();
		virtual void evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const = 0;
		virtual void evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const = 0;
		virtual void evalPDF(float &pdf) const = 0;
		virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) = 0;
		void setKdText(Texture * const kdText);
		void setKsText(Texture * const ksText);
		void setKeText(Texture * const keText);
		void setKd(const vec3 &v);
		void setKs(const vec3 &v);
		void setKe(const vec3 &v);
		Texture * getKdText();
		Texture * getKsText();
		Texture * getKeText();
		vec3 getKd() const;
		vec3 getKs() const;
		vec3 getKe() const;
		bool isRefractive() const;
	protected:
		Texture * m_kdText; // diffuse texture
		Texture * m_ksText; // specular texture
		Texture * m_keText; // emissive texture
		vec3 m_kd; // diffuse color
		vec3 m_ks; // specular color
		vec3 m_ke; // emissive color
		bool m_refr;
	};

}

#endif // MATERIAL_H
