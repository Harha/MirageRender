// std includes

// mirage includes
#include "material.h"

namespace mirage
{

Material::Material(vec3 kd, vec3 ks, vec3 ke) : m_kd(kd), m_ks(ks), m_ke(ke)
{

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

}
