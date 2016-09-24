#include "material.h"

// std includes

// mirage includes
#include "../macros.h"
#include "texture.h"

namespace mirage
{

	Material::Material(
		Texture * const kdText,
		Texture * const ksText,
		Texture * const keText,
		vec3 kd, vec3 ks, vec3 ke,
		bool refr) :
		m_kdText(kdText),
		m_ksText(ksText),
		m_keText(keText),
		m_kd(kd), m_ks(ks), m_ke(ke),
		m_refr(refr)
	{

	}

	Material::Material(
		const std::string & kdText,
		const std::string & ksText,
		const std::string & keText,
		vec3 kd, vec3 ks, vec3 ke,
		bool refr) :
		m_kdText(!kdText.empty() ? new Texture(kdText) : nullptr),
		m_ksText(!ksText.empty() ? new Texture(ksText) : nullptr),
		m_keText(!keText.empty() ? new Texture(keText) : nullptr),
		m_kd(kd), m_ks(ks), m_ke(ke),
		m_refr(refr)
	{

	}

	Material::~Material()
	{
		DELETE(m_kdText);
		DELETE(m_ksText);
		DELETE(m_keText);
	}

	void Material::setKdText(Texture * const kdText)
	{
		m_kdText = kdText;
	}

	void Material::setKsText(Texture * const ksText)
	{
		m_ksText = ksText;
	}

	void Material::setKeText(Texture * const keText)
	{
		m_keText = keText;
	}

	void Material::setKd(const vec3 &v)
	{
		m_kd = v;
	}

	void Material::setKs(const vec3 &v)
	{
		m_ks = v;
	}

	void Material::setKe(const vec3 &v)
	{
		m_ke = v;
	}

	Texture * Material::getKdText()
	{
		return m_kdText;
	}

	Texture * Material::getKsText()
	{
		return m_ksText;
	}

	Texture * Material::getKeText()
	{
		return m_keText;
	}

	vec3 Material::getKd() const
	{
		return m_kd;
	}

	vec3 Material::getKs() const
	{
		return m_ks;
	}

	vec3 Material::getKe() const
	{
		return m_ke;
	}

	bool Material::isRefractive() const
	{
		return m_refr;
	}

}
