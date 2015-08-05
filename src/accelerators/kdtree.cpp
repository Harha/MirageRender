// std includes
#include <iostream>
#include <algorithm>
#include <ctime>

// mirage includes
#include "kdtree.h"
#include "../macros.h"

namespace mirage
{

KDTreeAccel::KDTreeAccel(const Transform &o2w, const Transform &w2o, std::vector<Shape *> shapes,
                         const float iCost, const float tCost, const float maxP, const float maxD, const float lThreshold)
    : Accelerator(o2w, w2o, shapes), m_iSectCost(iCost), m_travCost(tCost),
      m_maxPrims(maxP), m_maxDepth(maxD), m_leafThreshold(lThreshold)
{
    LOG("a New k-d tree accelerator was constructed! Number of shapes: " << m_shapes.size());
}

void KDTreeAccel::update() const
{
    ERR("Unimplemented function KDTreeAccel::update called!");
}

bool KDTreeAccel::intersect(const Ray &ray, Intersection &iSect)
{
    bool result = false;
    float tHit = INFINITY;
    float tHit0 = 0.0f;
    float tHit1 = INFINITY;
    traverse(&m_root, ray, result, tHit, tHit0, tHit1, iSect);
    //if (result)
    //    LOG("Test!");
    return result;
}

bool KDTreeAccel::intersectP(const Ray &ray) const
{
    ERR("Unimplemented function KDTreeAccel::intersectP called!");
    return false;
}

void KDTreeAccel::build()
{
    LOG("Started building the k-d tree...");
    std::clock_t startTime = std::clock(); // Re-using my old code

    buildRecursive(&m_root, 0, m_shapes);

    std::clock_t endTime = std::clock();
    std::clock_t time = endTime - startTime;
    float duration = time / (double) CLOCKS_PER_SEC;

    LOG("K-d tree build finished! Time taken: " << duration);
}

void KDTreeAccel::buildRecursive(KDNode *node, int depth, std::vector<Shape *> &shapes)
{
    // Calculate node axis-aligned bounding box
    AABB node_bbox = shapes[0]->worldBound();
    for (size_t i = 0; i < shapes.size(); i++)
    {
        node_bbox = node_bbox.addBox(shapes[i]->worldBound());
    }
    node->aabb = node_bbox;

    // Split along the longest axis, find it...
    const int axis = node_bbox.getMaximumExtent();
    node->split_axis = axis;

    // Calculate split point & sort the primitives based on pre-set rules
    const int median = shapes.size() >> 1;
    std::nth_element(shapes.begin(), shapes.begin() + median, shapes.end(), KDCompareShapes(axis));
    node->split_pos = shapes[median]->worldBound().getCentroid()[axis];

    // Create a leaf
    if (shapes.size() <= m_leafThreshold || depth >= m_maxDepth)
    {
        node->data.insert(node->data.end(), shapes.begin(), shapes.end());
        //LOG("Leaf: " << node->data.size() << " | " << node->split_axis << " | " << node->split_pos);
        return;
    }

    // Create branches
    node->lChild = new KDNode;
    node->rChild = new KDNode;

    // Create left & right child shape vectors
    std::vector<Shape *> lList(median);
    std::vector<Shape *> rList(shapes.size() - median);
    std::copy(shapes.begin(), shapes.begin() + median, lList.begin());
    std::copy(shapes.begin() + median, shapes.end(), rList.begin());

    // Recursive call to build the child nodes
    buildRecursive(node->lChild, depth++, lList);
    buildRecursive(node->rChild, depth++, rList);
}

void KDTreeAccel::traverse(KDNode *node, const Ray &ray, bool &bHit, float &tHit, float &tHit0, float &tHit1, Intersection &iSect)
{
    if (node->isLeaf())
    {
        bool bInit = false;
        float tInit = INFINITY;
        for (auto *s : node->data)
        {
            Intersection iSectInit;
            bInit = s->intersect(ray, tInit, iSectInit);
            if (bInit && tInit < tHit)
            {
                bHit = bInit;
                tHit = tInit;
                iSect = iSectInit;
            }
        }
    }
    else
    {
        if (node->lChild->aabb.intersectP(ray, tHit0, tHit1))
        {
            traverse(node->lChild, ray, bHit, tHit, tHit0, tHit1, iSect);
        }
        if (node->rChild->aabb.intersectP(ray, tHit0, tHit1))
        {
            traverse(node->rChild, ray, bHit, tHit, tHit0, tHit1, iSect);
        }
    }
}

}
