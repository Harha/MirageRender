#ifndef OBJFACTORY_H
#define OBJFACTORY_H

// std includes
#include <vector>

namespace mirage
{

	struct vec3;
	struct vec4;
	struct mat4;
	struct quaternion;
	class Transform;
	class Camera;
	class Light;
	class Shape;
	class Mesh;
	class Material;

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
		Camera *initOrthoCamera(const Transform t, float speed, float sensitivity, float zoom, unsigned resX, unsigned resY);
		Camera *initPerspCamera(const Transform t, float speed, float sensitivity, float fov, unsigned resX, unsigned resY);
		Light *initDirLight(const Transform t, const vec3 emission);
		Light *initPointLight(const Transform t, const vec3 emission, float aC, float aL, float aQ);
		Light *initSpotLight(const Transform t, const vec3 emission, float aC, float aL, float aQ, float cutoff);
		Shape *initShape(Shape *s);
		Mesh *initMesh(Mesh *m);
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
		std::vector<Shape *> m_loadedShapes;
		std::vector<Mesh *> m_loadedMeshes;
		std::vector<Material *> m_loadedMaterials;
	};

}

#endif // OBJFACTORY_H
