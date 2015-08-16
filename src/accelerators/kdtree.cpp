// std includes
#include <iostream>
#include <algorithm>
#include <ctime>

// mirage includes
#include "kdtree.h"
#include "../macros.h"

namespace mirage
{

KDNode::KDNode(int axis, float pos, std::vector<Shape *> data, AABB bbox) : split_axis(axis), split_pos(pos), data(data), aabb(bbox)
{
    lChild = nullptr;
    rChild = nullptr;
}

KDNode::~KDNode()
{
    if (lChild)
        delete lChild;
    if (rChild)
        delete rChild;
}

bool KDNode::isLeaf()
{
    return (lChild == nullptr) && (rChild == nullptr);
}

KDTreeAccel::KDTreeAccel(std::vector<Shape *> shapes, const float iCost, const float tCost,
                         const float maxP, const float maxD, const float lThreshold)
    : Accelerator(shapes), m_iSectCost(iCost), m_travCost(tCost),
      m_maxPrims(maxP), m_maxDepth(maxD), m_leafThreshold(lThreshold)
{
    LOG("K-D Tree: a New K-D Tree accelerator object was created.");
    LOG("K-D Tree: Number of loaded shapes: " << m_shapes.size());
}

KDTreeAccel::~KDTreeAccel()
{
    if (m_root)
        delete m_root;
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
    traverse(m_root, ray, result, tHit, tHit0, tHit1, iSect);
    return result;
}

bool KDTreeAccel::intersectP(const Ray &ray)
{
    bool result = false;
    float tHit = ray.maxt;
    float tHit0 = ray.mint;
    float tHit1 = INFINITY;
    traverseP(m_root, ray, result, tHit, tHit0, tHit1);
    return result;
}

void KDTreeAccel::init()
{
    LOG("K-D Tree: Started building the tree...");
    std::clock_t startTime = std::clock();

    m_root = new KDNode;
    buildRecursive(m_root, 0, m_shapes);

    std::clock_t endTime = std::clock();
    std::clock_t time = endTime - startTime;
    float duration = time / (double) CLOCKS_PER_SEC;

    m_initialized = true;
    LOG("K-D Tree: Build finished! Time taken: " << duration << "s.");
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
    std::nth_element(shapes.begin(), shapes.begin() + median, shapes.end(), KDCompareShapes(axis));
    node->split_pos = shapes[median]->worldBound().getCentroid()[axis];

    // Create branches
    node->lChild = new KDNode;
    node->rChild = new KDNode;

    // Create left & right child shape vectors
    std::vector<Shape *> lList(median);
    std::vector<Shape *> rList(shapes.size() - median);
    std::copy(shapes.begin(), shapes.begin() + median, lList.begin());
    std::copy(shapes.begin() + median, shapes.end(), rList.begin());

    // Recursive call to build the child nodes
    buildRecursive(node->lChild, depth + 1, lList);
    buildRecursive(node->rChild, depth + 1, rList);
}

void KDTreeAccel::traverse(KDNode *node, const Ray &ray, bool &bHit, float &tHit, float &tHit0, float &tHit1, Intersection &iSect)
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
        for (auto *s : node->data)
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

void KDTreeAccel::traverseP(KDNode *node, const Ray &ray, bool &bHit, float &tHit, float &tHit0, float &tHit1)
{
    // Find first intersection in the leaf, don't care about z-sorting
    if (node->isLeaf())
    {
        float tInit = tHit1;
        Intersection iSectInit;
        for (auto *s : node->data)
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
        if (node->lChild->aabb.intersectP(ray, tHit0, tHit1))
        {
            traverseP(node->lChild, ray, bHit, tHit, tHit0, tHit1);
        }
        if (node->rChild->aabb.intersectP(ray, tHit0, tHit1))
        {
            traverseP(node->rChild, ray, bHit, tHit, tHit0, tHit1);
        }
    }
}

}
