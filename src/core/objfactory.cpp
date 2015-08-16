// std includes
#include <iostream>

// mirage includes
#include "objfactory.h"
#include "../macros.h"

namespace mirage
{

ObjFactory::ObjFactory(std::vector<Material *> materials) : m_loadedMaterials(materials)
{
    LOG("ObjFactory: a New ObjFactory object was created.");
}

ObjFactory::~ObjFactory()
{
    for (size_t i = 0; i < m_loadedMaterials.size(); i++)
    {
        Material *material = m_loadedMaterials[i];
        if (material)
        {
            delete material;

            LOG("ObjFactory: an Instance of (Material *) was destroyed succesfully...");
        }
    }
}

Material *ObjFactory::initDiffuseMaterial(const vec3 &kd, const vec3 &ke)
{
    m_loadedMaterials.push_back(new DiffuseMaterial(kd, ke));

    LOG("ObjFactory: Initialized a new DiffuseMaterial.");
    return m_loadedMaterials.back();
}

Material *ObjFactory::initGlassMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke, const float ior)
{
    m_loadedMaterials.push_back(new GlassMaterial(kd, ks, ke, ior));

    LOG("ObjFactory: Initialized a new GlassMaterial.");
    return m_loadedMaterials.back();
}

Material *ObjFactory::initGlossyMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke, const float r, const float k, const float d)
{
    m_loadedMaterials.push_back(new GlossyMaterial(kd, ks, ke, r, k, d));

    LOG("ObjFactory: Initialized a new GlossyMaterial.");
    return m_loadedMaterials.back();
}

Material *ObjFactory::initSpecularMaterial(const vec3 &kd, const vec3 &ks, const vec3 &ke)
{
    m_loadedMaterials.push_back(new SpecularMaterial(kd, ks, ke));

    LOG("ObjFactory: Initialized a new SpecularMaterial.");
    return m_loadedMaterials.back();
}

}
