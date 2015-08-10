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

            LOG("an Instance of DiffuseMaterial was destroyed succesfully...");
        }
    }

    LOG("an Instance of MatFactory has been destroyed succesfully.");
}

Material *MatFactory::initDiffuseMaterial()
{
    m_loadedMaterials.push_back(new DiffuseMaterial());

    LOG("Initialized a new DiffuseMaterial...");
    return m_loadedMaterials.back();
}

}
