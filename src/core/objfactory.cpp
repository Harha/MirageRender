// std includes
#include <iostream>

// mirage includes
#include "objfactory.h"
#include "../macros.h"
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
#include "shape.h"
#include "../shapes/mesh.h"
#include "material.h"
#include "../materials/diffusemat.h"
#include "../materials/dielectric.h"
#include "../materials/glossymat.h"
#include "../materials/specmat.h"
#include "texture.h"

namespace mirage
{

	ObjFactory::ObjFactory()
	{
		MLOG_INFO("ObjFactory: a new instance was initialized.");
	}

	ObjFactory::~ObjFactory()
	{
		for (size_t i = 0; i < m_loadedVector3s.size(); i++)
		{
			vec3 *vector3 = m_loadedVector3s[i];

			if (vector3)
			{
				delete vector3;

				MLOG_INFO("ObjFactory: an Instance of vec3 was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedVector4s.size(); i++)
		{
			vec4 *vector4 = m_loadedVector4s[i];

			if (vector4)
			{
				delete vector4;

				MLOG_INFO("ObjFactory: an Instance of vec4 was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedQuaternions.size(); i++)
		{
			quaternion *quat = m_loadedQuaternions[i];

			if (quat)
			{
				delete quat;

				MLOG_INFO("ObjFactory: an Instance of quaternion was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedTransforms.size(); i++)
		{
			Transform *trans = m_loadedTransforms[i];

			if (trans)
			{
				delete trans;

				MLOG_INFO("ObjFactory: an Instance of Transform was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedCameras.size(); i++)
		{
			Camera *cam = m_loadedCameras[i];

			if (cam)
			{
				delete cam;

				MLOG_INFO("ObjFactory: an Instance of Camera was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedLights.size(); i++)
		{
			Light *light = m_loadedLights[i];

			if (light)
			{
				delete light;

				MLOG_INFO("ObjFactory: an Instance of Light was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedShapes.size(); i++)
		{
			Shape *shape = m_loadedShapes[i];

			if (shape)
			{
				delete shape;

				MLOG_INFO("ObjFactory: an Instance of Shape was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedMeshes.size(); i++)
		{
			Mesh *mesh = m_loadedMeshes[i];

			if (mesh)
			{
				delete mesh;

				MLOG_INFO("ObjFactory: an Instance of Mesh was destroyed.");
			}
		}

		for (size_t i = 0; i < m_loadedMaterials.size(); i++)
		{
			Material *material = m_loadedMaterials[i];
			if (material)
			{
				delete material;

				MLOG_INFO("ObjFactory: an Instance of Material was destroyed.");
			}
		}
	}

	vec3 * ObjFactory::initVector3(float x, float y, float z)
	{
		vec3 *v = new vec3(x, y, z);
		m_loadedVector3s.push_back(v);

		MLOG_INFO("ObjFactory: Allocated a new %s.", v->toString().c_str());
		return v;
	}

	vec4 * ObjFactory::initVector4(float x, float y, float z, float w)
	{
		vec4 *v = new vec4(x, y, z, w);
		m_loadedVector4s.push_back(v);

		MLOG_INFO("ObjFactory: Allocated a new %s.", v->toString().c_str());
		return v;
	}

	quaternion * ObjFactory::initQuaternion(float w, float x, float y, float z)
	{
		quaternion *q = new quaternion(w, x, y, z);
		m_loadedQuaternions.push_back(q);

		MLOG_INFO("ObjFactory: Allocated a new %s.", q->toString().c_str());
		return q;
	}

	quaternion * ObjFactory::initQuaternion(const vec3 origin, const vec3 target)
	{
		quaternion *q = new quaternion();
		*q = q->identity().lookAt(origin, target);
		m_loadedQuaternions.push_back(q);

		MLOG_INFO("ObjFactory: Allocated a new %s.", q->toString().c_str());
		return q;
	}

	Transform * ObjFactory::initTransform(const vec3 p, const quaternion o, const vec3 s)
	{
		m_loadedTransforms.push_back(new Transform(p, o, s));

		MLOG_INFO("ObjFactory: Allocated a new Transform.");
		return m_loadedTransforms.back();
	}

	Camera * ObjFactory::initOrthoCamera(const Transform t, float speed, float sensitivity, float zoom, unsigned resX, unsigned resY)
	{
		m_loadedCameras.push_back(new CameraOrtho(t, Film(resX, resY), speed, sensitivity, zoom));

		MLOG_INFO("ObjFactory: Allocated a new orthographic camera.");
		return m_loadedCameras.back();
	}

	Camera * ObjFactory::initPerspCamera(const Transform t, float speed, float sensitivity, float fov, unsigned resX, unsigned resY)
	{
		m_loadedCameras.push_back(new CameraPersp(t, Film(resX, resY), speed, sensitivity, fov));

		MLOG_INFO("ObjFactory: Allocated a new perspective camera.");
		return m_loadedCameras.back();
	}

	Light * ObjFactory::initDirLight(const Transform t, const vec3 emission)
	{
		m_loadedLights.push_back(new DirectionalLight(t, emission));

		MLOG_INFO("ObjFactory: Allocated a new directional light.");
		return m_loadedLights.back();
	}

	Light * ObjFactory::initPointLight(const Transform t, const vec3 emission, float aC, float aL, float aQ)
	{
		m_loadedLights.push_back(new PointLight(t, emission, aC, aL, aQ));

		MLOG_INFO("ObjFactory: Allocated a new point light.");
		return m_loadedLights.back();
	}

	Light * ObjFactory::initSpotLight(const Transform t, const vec3 emission, float aC, float aL, float aQ, float cutoff)
	{
		m_loadedLights.push_back(new SpotLight(t, emission, aC, aL, aQ, cutoff));

		MLOG_INFO("ObjFactory: Allocated a new spot light.");
		return m_loadedLights.back();
	}

	Shape * ObjFactory::initShape(Shape *s)
	{
		m_loadedShapes.push_back(s);

		MLOG_INFO("ObjFactory: Allocated a new shape.");
		return m_loadedShapes.back();
	}

	Mesh * ObjFactory::initMesh(Mesh *m)
	{
		m_loadedMeshes.push_back(m);

		MLOG_INFO("ObjFactory: Allocated a new mesh.");
		return m_loadedMeshes.back();
	}

	Material * ObjFactory::initDiffuseMaterial(const std::string & kdText, const vec3 kd, const vec3 ke)
	{
		m_loadedMaterials.push_back(new DiffuseMaterial(kdText, kd, ke));

		MLOG_INFO("ObjFactory: Allocated a new diffuse material.");
		return m_loadedMaterials.back();
	}

	Material * ObjFactory::initDielectricMaterial(const vec3 kd, const vec3 ks, const vec3 ke, const float ior)
	{
		m_loadedMaterials.push_back(new DielectricMaterial(kd, ks, ke, ior));

		MLOG_INFO("ObjFactory: Allocated a new dielectric material.");
		return m_loadedMaterials.back();
	}

	Material * ObjFactory::initGlossyMaterial(const vec3 kd, const vec3 ks, const vec3 ke, const float r, const float k, const float d)
	{
		m_loadedMaterials.push_back(new GlossyMaterial(kd, ks, ke, r, k, d));

		MLOG_INFO("ObjFactory: Allocated a new glossy material.");
		return m_loadedMaterials.back();
	}

	Material * ObjFactory::initSpecularMaterial(const vec3 kd, const vec3 ks, const vec3 ke)
	{
		m_loadedMaterials.push_back(new SpecularMaterial(kd, ks, ke));

		MLOG_INFO("ObjFactory: Allocated a new specular material.");
		return m_loadedMaterials.back();
	}

}
