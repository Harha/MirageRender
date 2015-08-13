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
    Material(vec3 kd = vec3(), vec3 ks = vec3(), vec3 ke = vec3());
    virtual ~Material();
    virtual void evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const = 0;
    virtual void evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const = 0;
    virtual void evalPDF(float &pdf) const = 0;
    virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) = 0;
    void setKd(const vec3 &v);
    void setKs(const vec3 &v);
    void setKe(const vec3 &v);
    vec3 getKd() const;
    vec3 getKs() const;
    vec3 getKe() const;
protected:
    vec3 m_kd; // diffuse
    vec3 m_ks; // specular
    vec3 m_ke; // emittance
};

}

#endif // MATERIAL_H
