#ifndef ACCELERATOR_H
#define ACCELERATOR_H

// mirage includes
#include "shape.h"

namespace mirage
{

	class Accelerator
	{
	public:
		Accelerator(const std::vector<Shape *> shapes = std::vector<Shape *>());
		virtual ~Accelerator();
		virtual void update() const = 0;
		virtual AABB objectBound() const;
		virtual AABB worldBound() const;
		virtual bool intersect(const Ray & ray, Intersection & iSect) = 0;
		virtual bool intersectP(const Ray & ray) = 0;
		virtual void init() = 0;
	private:
	protected:
		std::vector<Shape *> m_shapes;
		bool m_initialized;
	};

}

#endif // ACCELERATOR_H
