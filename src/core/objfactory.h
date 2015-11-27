#ifndef OBJFACTORY_H
#define OBJFACTORY_H

// std includes
#include <vector>

// mirage includes
#include "../math/vec3.h"
#include "../math/vec4.h"
#include "../math/mat4.h"
#include "../math/quaternion.h"
#include "transform.h"
#include "camera.h"
#include "../cameras/orthographic.h"
#include "../cameras/perspective.h"
#include "light.h"
#include "../lights/dirlight.h"
#include "../lights/pointlight.h"
#include "../lights/spotlight.h"
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
    ObjFactory();
    ~ObjFactory();

    vec3 *initVector3(float x, float y, float z);
    vec4 *initVector4(float x, float y, float z, float w);
    quaternion *initQuaternion(float w, float x, float y, float z);
    quaternion *initQuaternion(const vec3 origin, const vec3 target);
    Transform *initTransform(const vec3 p, const quaternion o, const vec3 s);
    Camera *initOrthoCamera(const Transform t, float speed, float sensitivity, float zoom);
    Camera *initPerspCamera(const Transform t, float speed, float sensitivity, float fov);
    Light *initDirLight(const Transform t, const vec3 emission);
    Light *initPointLight(const Transform t, const vec3 emission, float aC, float aL, float aQ);
    Light *initSpotLight(const Transform t, const vec3 emission, float aC, float aL, float aQ, float cutoff);
    Material *initDiffuseMaterial(const vec3 kd, const vec3 ke);
    Material *initGlassMaterial(const vec3 kd, const vec3 ks, const vec3 ke, const float ior);
    Material *initGlossyMaterial(const vec3 kd, const vec3 ks, const vec3 ke, const float r, const float k, const float d);
    Material *initSpecularMaterial(const vec3 kd, const vec3 ks, const vec3 ke);
private:
    std::vector<vec3 *> m_loadedVector3s;
    std::vector<vec4 *> m_loadedVector4s;
    std::vector<quaternion *> m_loadedQuaternions;
    std::vector<Transform *> m_loadedTransforms;
    std::vector<Camera *> m_loadedCameras;
    std::vector<Light *> m_loadedLights;
    std::vector<Material *> m_loadedMaterials;
};

}

#endif // OBJFACTORY_H
