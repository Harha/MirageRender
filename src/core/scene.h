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

	class Display;

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
		void addShape(Shape *s);
		void setRadianceClamping(float f);
		void setMaxRecursion(int n);
		void setSkyColor(const vec3 & c);
		Accelerator *getAccelerator() const;
		ObjFactory *getObjFactory() const;
		Camera *getCamera() const;
		std::vector<Light *> getLights() const;
		std::vector<Mesh *> getMeshes() const;
		std::vector<Shape *> getShapes() const;
		float getRadianceClamping() const;
		int getMaxRecursion() const;
		vec3 getSkyColor() const;
	private:
		Accelerator *m_accelerator;
		ObjFactory *m_objFactory;
		std::vector<Camera *> m_cameras;
		std::vector<Light *> m_lights;
		std::vector<Mesh *> m_meshes;
		std::vector<Shape *> m_shapes;
		float m_radianceClamping;
		int m_maxRecursion;
		vec3 m_skyColor;
	};

}

#endif // SCENE_H
