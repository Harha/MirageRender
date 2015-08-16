#ifndef MESH_H
#define MESH_H

// std includes
#include <string>
#include <map>

// mirage includes
#include "../core/shape.h"
#include "../core/vertex.h"
#include "../core/material.h"
#include "../core/objfactory.h"
#include "../materials/diffusemat.h"
#include "triangle.h"

namespace mirage
{

struct face
{
    int va, vb, vc; // vertex indices
    int na, nb, nc; // normal indices
    std::string material;
};

class Mesh : public virtual Shape
{
public:
    Mesh(const Transform o2w, Material *m = nullptr, ObjFactory *objFactory = nullptr, std::string fileName = "null");
    virtual void update() override;
    virtual AABB objectBound() const override;
    virtual AABB worldBound() const override;
    virtual bool intersect(const Ray &ray, Intersection &iSect) const override;
    virtual bool intersectP(const Ray &ray) const override;
    virtual float getSurfaceArea() const override;
    std::vector<Shape *> getShapes();
private:
    int loadObj();
    int loadMTL(std::map<std::string, Material *> &materials);

    ObjFactory *m_objFactory;
    std::string m_mdlFileName;
    std::string m_mtlFileName;
    std::vector<Triangle> m_triangles;
};

}

#endif // MESH_H
