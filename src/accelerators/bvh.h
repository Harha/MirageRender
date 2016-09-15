#ifndef BVH_H
#define BVH_H

// mirage includes
#include "../core/accelerator.h"

namespace mirage
{

	// ------------------------------------------------------------------------
	// BVH Shape Comparator
	// ------------------------------------------------------------------------
	struct BVHCompareShapes
	{
		BVHCompareShapes(const int axis = 0) : split_axis(axis)
		{

		}

		bool operator()(const Shape *a, const Shape *b) const
		{
			return a->worldBound().getCentroid()[split_axis] < b->worldBound().getCentroid()[split_axis];
		}

		int split_axis;
	};

	// ------------------------------------------------------------------------
	// BVH Node Object
	// ------------------------------------------------------------------------
	struct BVHNode
	{
		BVHNode(
			int axis = 0,
			float pos = 0.0f,
			std::vector<Shape *> data = std::vector<Shape *>(),
			AABB bbox = AABB()
		);
		~BVHNode();

		bool isLeaf();

		int split_axis;
		float split_pos;
		std::vector<Shape *> data;
		AABB aabb;
		BVHNode * l_child;
		BVHNode * r_child;
	};

	// ------------------------------------------------------------------------
	// BVH Accelerator Object
	// ------------------------------------------------------------------------
	class BVHAccel : public virtual Accelerator
	{
	public:
		BVHAccel(
			const std::vector<Shape *> shapes = std::vector<Shape *>(),
			const float lThreshold = 1
		);
		~BVHAccel();

		virtual void update() const override;
		virtual bool intersect(const Ray & ray, Intersection &iSect) override;
		virtual bool intersectP(const Ray & ray) override;
		virtual void init() override;
		void buildRecursive(BVHNode * node, int depth, std::vector<Shape *> & shapes);
		void traverse(BVHNode * node, const Ray & ray, bool & bHit, float & tHit, float & tHit0, float & tHit1, Intersection & iSect);
		void traverseP(BVHNode * node, const Ray & ray, bool & bHit, float & tHit, float & tHit0, float & tHit1);
	private:
		int m_iSectCost;
		int m_travCost;
		int m_maxPrims;
		int m_maxDepth;
		unsigned m_leafThreshold;
		BVHNode *m_root;
	};

}

#endif // BVH_H
