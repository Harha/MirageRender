#ifndef SCENE_H
#define SCENE_H

// std includes
#include <string>
#include <vector>

// mirage includes
#include "accelerator.h"
#include "camera.h"
#include "objfactory.h"
#include "ray.h"
#include "intersection.h"
#include "light.h"
#include "../shapes/mesh.h"

namespace mirage
{

class Scene
{
public:
    Scene();
    ~Scene();
    bool intersect(const Ray &ray, Intersection &iSect) const;
    bool intersectP(const Ray &ray) const;
    void setAccelerator(Accelerator *accel);
    void setObjFactory(ObjFactory *objfac);
    void addCamera(Camera *c);
    void addLight(Light *l);
    void addMesh(Mesh *m);
    Accelerator *getAccelerator() const;
    ObjFactory *getObjFactory() const;
    Camera *getCamera() const;
    std::vector<Light *> getLights() const;
    float getRadClamping() const;
    int getRecMax() const;
private:
    Accelerator *m_accelerator;
    ObjFactory *m_objFactory;
    std::vector<Camera *> m_cameras;
    std::vector<Light *> m_lights;
    std::vector<Mesh *> m_meshes;
    float m_radClamping;
    int m_recMax;
};

}

#endif // SCENE_H
