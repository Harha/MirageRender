#ifndef KDTREE_H
#define KDTREE_H

// mirage includes
#include "../core/accelerator.h"

namespace mirage
{

struct KDCompareShapes
{
    KDCompareShapes(const int axis = 0) : split_axis(axis)
    {

    }

    bool operator()(const Shape *a, const Shape *b) const
    {
        return a->worldBound().getCentroid()[split_axis] < b->worldBound().getCentroid()[split_axis];
    }

    int split_axis;
};

struct KDNode
{
    KDNode(int axis = 0.0f, float pos = 0, std::vector<Shape *> data = std::vector<Shape *>(), AABB bbox = AABB());
    ~KDNode();
    bool isLeaf();

    int split_axis;
    float split_pos;
    std::vector<Shape *> data;
    AABB aabb;
    KDNode *lChild;
    KDNode *rChild;
};

class KDTreeAccel : public Accelerator
{
public:
    KDTreeAccel(const Transform &o2w = Transform(), const Transform &w2o = Transform(), const std::vector<Shape *> shapes = std::vector<Shape *>(),
                const float iCost = 1, const float tCost = 1, const float maxP = 1028, const float maxD = 5, const float lThreshold = 1);
    ~KDTreeAccel();
    virtual void update() const override;
    virtual bool intersect(const Ray &ray, Intersection &iSect) override;
    virtual bool intersectP(const Ray &ray) const override;
    virtual void init() override;
    void buildRecursive(KDNode *node, int depth, std::vector<Shape *> &shapes);
    void traverse(KDNode *node, const Ray &ray, bool &bHit, float &tHit, float &tHit0, float &tHit1, Intersection &iSect);
private:
    int m_iSectCost;
    int m_travCost;
    int m_maxPrims;
    int m_maxDepth;
    int m_leafThreshold;
    KDNode *m_root;
};

}

#endif // KDTREE_H
