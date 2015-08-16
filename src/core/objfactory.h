#ifndef OBJFACTORY_H
#define OBJFACTORY_H

// std includes
#include <vector>

// mirage includes
#include "material.h"
#include "../materials/diffusemat.h"
#include "../materials/glassmat.h"
#include "../materials/glossymat.h"
#include "../materials/specmat.h"

namespace mirage
{

class ObjFactory
{
public:
    ObjFactory(std::vector<Material *> materials = std::vector<Material *>());
    ~ObjFactory();
    Material *initDiffuseMaterial(const vec3 &kd, const vec3 &ke);
    Material *initGlassMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke, const float ior);
    Material *initGlossyMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke, const float r, const float k, const float d);
    Material *initSpecularMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke);
private:
    std::vector<Material *> m_loadedMaterials;
};

}

#endif // OBJFACTORY_H
