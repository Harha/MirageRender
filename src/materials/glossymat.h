#ifndef GLOSSY_H
#define GLOSSY_H

// std includes

// mirage includes
#include "../core/material.h"

namespace mirage
{

class GlossyMaterial : public virtual Material
{
public:
    GlossyMaterial(vec3 kd = vec3(), vec3 ks = vec3(), vec3 ke = vec3(), float r = 0.1f, float k = 0.9f, float d = 0.9f);
    virtual ~GlossyMaterial() override;
    virtual void evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const override;
    virtual void evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const override;
    virtual void evalPDF(float &pdf) const override;
    virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) override;
private:
    float m_r;
    float m_k;
    float m_d;
};

}

#endif // GLOSSY_H
