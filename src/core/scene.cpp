// std includes
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <map>

// mirage includes
#include "scene.h"
#include "../macros.h"
#include "transform.h"
#include "../accelerators/kdtree.h"
#include "../cameras/orthographic.h"
#include "../cameras/perspective.h"
#include "light.h"
#include "../lights/pointlight.h"

namespace mirage
{

Scene::Scene() : m_accelerator(nullptr), m_objFactory(new ObjFactory()), m_radClamping(100.0f), m_recMax(5)
{
    LOG("Scene: a New Scene object was created.");
}

Scene::~Scene()
{
    /* Destroy heap objects in order */
    DELETE(m_accelerator);
    DELETE(m_objFactory);
}

bool Scene::intersect(const Ray &ray, Intersection &iSect) const
{
    return m_accelerator->intersect(ray, iSect);
}

bool Scene::intersectP(const Ray &ray) const
{
    return m_accelerator->intersectP(ray);
}

void Scene::setAccelerator(Accelerator *accel)
{
    m_accelerator = accel;
}

void Scene::setObjFactory(ObjFactory *objfac)
{
    m_objFactory = objfac;
}

void Scene::addCamera(Camera *c)
{
    m_cameras.push_back(c);
}

void Scene::addLight(Light *l)
{
    m_lights.push_back(l);
}

void Scene::addMesh(Mesh *m)
{
    m_meshes.push_back(m);
}

Accelerator *Scene::getAccelerator() const
{
    return m_accelerator;
}

ObjFactory *Scene::getObjFactory() const
{
    return m_objFactory;
}

Camera *Scene::getCamera() const
{
    if (m_cameras.size() > 0)
    {
        return m_cameras.back();
    }
    else
    {
        return nullptr;
    }
}

std::vector<Light *> Scene::getLights() const
{
    return m_lights;
}

float Scene::getRadClamping() const
{
    return m_radClamping;
}

int Scene::getRecMax() const
{
    return m_recMax;
}

}
