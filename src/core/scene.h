#ifndef SCENE_H
#define SCENE_H

// std includes
#include <vector>

// mirage includes
#include "accelerator.h"
#include "camera.h"
#include "ray.h"
#include "intersection.h"
#include "light.h"

namespace mirage
{

class Scene
{
public:
    Scene(Accelerator *accel = nullptr, Camera *cam = nullptr);
    bool intersect(const Ray &ray, Intersection &iSect) const;
    bool intersectP(const Ray &ray) const;
    void addLight(Light *l);
    Accelerator *getAccelerator() const;
    Camera *getCamera() const;
    std::vector<Light *> getLights() const;
private:
    Accelerator *m_accelerator;
    Camera *m_camera;
    std::vector<Light *> m_lights;
};

}

#endif // SCENE_H
