// std includes
#include <iostream>

// mirage includes
#include "scene.h"
#include "../macros.h"

namespace mirage
{

Scene::Scene(Accelerator *accel, Camera *cam) : m_accelerator(accel), m_camera(cam)
{
    LOG("Scene: a New Scene object was created.");
}

bool Scene::intersect(const Ray &ray, Intersection &iSect) const
{
    return m_accelerator->intersect(ray, iSect);
}

bool Scene::intersectP(const Ray &ray) const
{
    return m_accelerator->intersectP(ray);
}

void Scene::addLight(Light *l)
{
    m_lights.push_back(l);
}

Accelerator *Scene::getAccelerator() const
{
    return m_accelerator;
}

Camera *Scene::getCamera() const
{
    return m_camera;
}

std::vector<Light *> Scene::getLights() const
{
    return m_lights;
}

}
