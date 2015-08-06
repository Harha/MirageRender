#ifndef MATERIAL_H
#define MATERIAL_H

// std includes

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

class Material
{
public:
    Material(vec3 kd = vec3(1, 1, 1), vec3 ks = vec3(1, 1, 1), vec3 ke = vec3());
    void setKd(const vec3 &v);
    void setKs(const vec3 &v);
    void setKe(const vec3 &v);
    vec3 getKd() const;
    vec3 getKs() const;
    vec3 getKe() const;
private:
    vec3 m_kd; // diffuse
    vec3 m_ks; // specular
    vec3 m_ke; // emittance
};

}

#endif // MATERIAL_H
