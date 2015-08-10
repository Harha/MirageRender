#ifndef MATFACTORY_H
#define MATFACTORY_H

// std includes
#include <vector>

// mirage includes
#include "material.h"
#include "../materials/diffusemat.h"

namespace mirage
{

class MatFactory
{
public:
    MatFactory(std::vector<Material *> materials = std::vector<Material *>());
    ~MatFactory();
    Material *initDiffuseMaterial();
private:
    std::vector<Material *> m_loadedMaterials;
};

}

#endif // MATFACTORY_H
