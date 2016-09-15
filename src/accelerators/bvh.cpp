#include "bvh.h"

// std includes
#include <iostream>
#include <algorithm>
#include <ctime>

// mirage includes
#include "../macros.h"

namespace mirage
{

	// ------------------------------------------------------------------------
	// BVH Node Object
	// ------------------------------------------------------------------------
	BVHNode::BVHNode(int axis, float pos, std::vector<Shape *> data, AABB bbox) :
		split_axis(axis),
		split_pos(pos),
		data(data),
		aabb(bbox),
		l_child(nullptr),
		r_child(nullptr)
	{
	}

	BVHNode::~BVHNode()
	{
		DELETE(l_child);
		DELETE(r_child);
	}

	bool BVHNode::isLeaf()
	{
		return (l_child == nullptr) && (r_child == nullptr);
	}

	// ------------------------------------------------------------------------
	// BVH Accelerator Object
	// ------------------------------------------------------------------------
	BVHAccel::BVHAccel(std::vector<Shape *> shapes, const float lThreshold) :
		Accelerator(shapes),
		m_leafThreshold(lThreshold)
	{
		LOG("BVHAccel: a New instance was created.");
		LOG("BVHAccel: Number of loaded shapes: " << m_shapes.size());
	}

	BVHAccel::~BVHAccel()
	{
		DELETE(m_root);
	}

	void BVHAccel::update() const
	{
		ERR("Unimplemented function BVHAccel::update was called.");
	}

	bool BVHAccel::intersect(const Ray &ray, Intersection &iSect)
	{
		bool result = false;
		float tHit = INFINITY;
		float tHit0 = 0.0f;
		float tHit1 = INFINITY;
		traverse(m_root, ray, result, tHit, tHit0, tHit1, iSect);
		return result;
	}

	bool BVHAccel::intersectP(const Ray &ray)
	{
		bool result = false;
		float tHit = ray.maxt;
		float tHit0 = ray.mint;
		float tHit1 = INFINITY;
		traverseP(m_root, ray, result, tHit, tHit0, tHit1);
		return result;
	}

	void BVHAccel::init()
	{
		LOG("BVHAccel: Started building the hierarchy...");
		std::clock_t startTime = std::clock();

		m_root = new BVHNode;
		buildRecursive(m_root, 0, m_shapes);

		std::clock_t endTime = std::clock();
		std::clock_t time = endTime - startTime;
		float duration = time / (double)CLOCKS_PER_SEC;

		m_initialized = true;
		LOG("BVHAccel: Build finished! Time taken: " << duration << "s.");
	}

	void BVHAccel::buildRecursive(BVHNode *node, int depth, std::vector<Shape *> &shapes)
	{
		// Calculate node axis-aligned bounding box
		AABB node_bbox = shapes[0]->worldBound();
		for (size_t i = 0; i < shapes.size(); i++)
		{
			node_bbox = node_bbox.addBox(shapes[i]->worldBound());
		}
		node->aabb = node_bbox;

		// Create a leaf if recursion limit was reached
		if (shapes.size() <= m_leafThreshold)
		{
			node->data.insert(node->data.end(), shapes.begin(), shapes.end());
			return;
		}

		// Split along the longest axis, find it...
		const int axis = node_bbox.getMaximumExtent();
		node->split_axis = axis;

		// Calculate split point & sort the primitives based on pre-set rules
		const int median = shapes.size() >> 1;
		std::nth_element(shapes.begin(), shapes.begin() + median, shapes.end(), BVHCompareShapes(axis));
		node->split_pos = shapes[median]->worldBound().getCentroid()[axis];

		// Create branches
		node->l_child = new BVHNode;
		node->r_child = new BVHNode;

		// Create left & right child shape vectors
		std::vector<Shape *> lList(median);
		std::vector<Shape *> rList(shapes.size() - median);
		std::copy(shapes.begin(), shapes.begin() + median, lList.begin());
		std::copy(shapes.begin() + median, shapes.end(), rList.begin());

		// Recursive call to build the child nodes
		buildRecursive(node->l_child, depth + 1, lList);
		buildRecursive(node->r_child, depth + 1, rList);
	}

	void BVHAccel::traverse(BVHNode * node, const Ray & ray, bool & bHit, float & tHit, float & tHit0, float & tHit1, Intersection & iSect)
	{
		// Intersect against each shape in the node if it's a leaf node
		if (node->isLeaf())
		{
			if (tHit0 > tHit)
			{
				return;
			}

			bool bInit = false;
			Intersection iSectInit;
			for (auto * s : node->data)
			{
				bInit = s->intersect(ray, iSectInit);
				if (bInit && iSectInit.getT() < tHit && iSectInit.getT() < ray.maxt)
				{
					bHit = true;
					tHit = iSectInit.getT();
					iSect = iSectInit;
				}
			}
		}
		// Otherwise intersect & possibly traverse child nodes
		else
		{
			if (node->l_child->aabb.intersectP(ray, tHit0, tHit1))
			{
				traverse(node->l_child, ray, bHit, tHit, tHit0, tHit1, iSect);
			}
			if (node->r_child->aabb.intersectP(ray, tHit0, tHit1))
			{
				traverse(node->r_child, ray, bHit, tHit, tHit0, tHit1, iSect);
			}
		}
	}

	void BVHAccel::traverseP(BVHNode * node, const Ray & ray, bool & bHit, float & tHit, float & tHit0, float & tHit1)
	{
		// Find first intersection in the leaf, don't care about z-sorting
		if (node->isLeaf())
		{
			float tInit = tHit1;
			Intersection iSectInit;
			for (auto * s : node->data)
			{
				if (s->intersectP(ray) && tInit < tHit && tInit < ray.maxt)
				{
					bHit = true;
					tHit = tInit;
				}
			}
		}
		// Otherwise intersect & possibly traverse child nodes
		else
		{
			if (node->l_child->aabb.intersectP(ray, tHit0, tHit1))
			{
				traverseP(node->l_child, ray, bHit, tHit, tHit0, tHit1);
			}
			if (node->r_child->aabb.intersectP(ray, tHit0, tHit1))
			{
				traverseP(node->r_child, ray, bHit, tHit, tHit0, tHit1);
			}
		}
	}

}
