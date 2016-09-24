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
#include "../accelerators/bvh.h"
#include "../cameras/orthographic.h"
#include "../cameras/perspective.h"
#include "light.h"
#include "../lights/pointlight.h"

namespace mirage
{

	Scene::Scene() :
		m_accelerator(nullptr),
		m_objFactory(new ObjFactory()),
		m_radianceClamping(100.0f),
		m_maxRecursion(5),
		m_skyColor(vec3(0.0f, 0.0f, 0.0f))
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

	void Scene::addShape(Shape *s)
	{
		m_shapes.push_back(s);
	}

	void Scene::setRadianceClamping(float f)
	{
		m_radianceClamping = f;
	}

	void Scene::setMaxRecursion(int n)
	{
		m_maxRecursion = n;
	}

	void Scene::setSkyColor(const vec3 & c)
	{
		m_skyColor = c;
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

	std::vector<Mesh *> Scene::getMeshes() const
	{
		return m_meshes;
	}

	std::vector<Shape *> Scene::getShapes() const
	{
		std::vector<Shape *> result;

		for (auto *m : m_meshes)
		{
			std::vector<Shape *> mesh_shapes = m->getShapes();
			for (size_t i = 0; i < mesh_shapes.size(); i++)
			{
				result.push_back(mesh_shapes[i]);
			}
		}

		for (size_t i = 0; i < m_shapes.size(); i++)
		{
			result.push_back(m_shapes[i]);
		}

		return result;
	}

	float Scene::getRadianceClamping() const
	{
		return m_radianceClamping;
	}

	int Scene::getMaxRecursion() const
	{
		return m_maxRecursion;
	}

	vec3 Scene::getSkyColor() const
	{
		return m_skyColor;
	}

}
