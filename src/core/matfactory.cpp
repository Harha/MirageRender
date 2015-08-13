// std includes
#include <iostream>

// mirage includes
#include "matfactory.h"
#include "../macros.h"

namespace mirage
{

MatFactory::MatFactory(std::vector<Material *> materials) : m_loadedMaterials(materials)
{
    LOG("a new Instance of MatFactory was created.");
}

MatFactory::~MatFactory()
{
    for (size_t i = 0; i < m_loadedMaterials.size(); i++)
    {
        Material *material = m_loadedMaterials[i];
        if (material)
        {
            delete material;

            LOG("an Instance of Material * was destroyed succesfully...");
        }
    }

    LOG("an Instance of MatFactory has been destroyed succesfully.");
}

Material *MatFactory::initDiffuseMaterial(const vec3 &kd, const vec3 &ke)
{
    m_loadedMaterials.push_back(new DiffuseMaterial(kd, ke));

    LOG("Initialized a new DiffuseMaterial...");
    return m_loadedMaterials.back();
}

Material *MatFactory::initGlassMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke, const float ior)
{
    m_loadedMaterials.push_back(new GlassMaterial(kd, ks, ke, ior));

    LOG("Initialized a new GlassMaterial...");
    return m_loadedMaterials.back();
}

Material *MatFactory::initGlossyMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke, const float r, const float k, const float d)
{
    m_loadedMaterials.push_back(new GlossyMaterial(kd, ks, ke, r, k, d));

    LOG("Initialized a new GlossyMaterial...");
    return m_loadedMaterials.back();
}

Material *MatFactory::initSpecularMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke)
{
    m_loadedMaterials.push_back(new SpecularMaterial());

    LOG("Initialized a new SpecularMaterial...");
    return m_loadedMaterials.back();
}

}
