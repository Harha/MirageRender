// std includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// mirage includes
#include "mesh.h"
#include "../macros.h"

namespace mirage
{

Mesh::Mesh(const Transform &o2w, const Transform &w2o, Material *m, MatFactory *matFactory, std::string fileName) : Shape(o2w, w2o, m), m_matFactory(matFactory), m_mdlFileName(fileName)
{
    if (!m_matFactory)
    {
        ERR("Supplied instance MatFactory is not loaded! Exiting program...");
        std::exit(1);
    }

    if (loadObj() != 0)
    {
        ERR("Error loading: " << m_mdlFileName);
        std::exit(1);
    }

    LOG("Mesh (" << m_mdlFileName << ") has been loaded succesfully. Triangles: " << m_triangles.size());
}

void Mesh::update()
{
    if (m_objToWorld.reqStateUpdate())
    {
        for (size_t i = 0; i < m_triangles.size(); i++)
        {
            m_triangles[i].update();
        }
    }
}

AABB Mesh::objectBound() const
{
    ERR("Called unimplemented method Mesh::objectBound!");
    return AABB();
}

AABB Mesh::worldBound() const
{
    ERR("Called unimplemented method Mesh::worldBound!");
    return AABB();
}

bool Mesh::intersect(const Ray &ray, Intersection &iSect) const
{
    ERR("Unimplemented function Mesh::intersect was called!");
    return false;
}

bool Mesh::intersectP(const Ray &ray) const
{
    ERR("Unimplemented function Mesh::intersectP was called!");
    return false;
}

float Mesh::getSurfaceArea() const
{
    return 0.0f;
}

std::vector<Shape *> Mesh::getShapes()
{
    std::vector<Shape *> result;
    result.reserve(m_triangles.size());
    for (size_t i = 0; i < m_triangles.size(); i++)
    {
        result.push_back(&m_triangles[i]);
    }
    return result;
}

int Mesh::loadObj()
{
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<face> indices;
    std::map<std::string, Material *> materials;
    std::string str_currentMaterial = "";

    std::ifstream file;
    std::string line;

    LOG("Attempting to load: " << m_mdlFileName);

    file.open("res/" + m_mdlFileName);

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            if (line.substr(0, 7) == "mtllib ")
            {
                m_mtlFileName = line.substr(7);

                if (loadMTL(materials) != 0)
                {
                    ERR("Error loading: " << m_mtlFileName);
                    std::exit(1);
                }
            }
            else if (line.substr(0, 7) == "usemtl " && !materials.empty())
            {
                str_currentMaterial = line.substr(7);
            }
            else if (line.substr(0, 2) == "v ")
            {
                std::istringstream s(line.substr(2));
                vec3 v;
                s >> v.x;
                s >> v.y;
                s >> v.z;
                vertices.push_back(v);
            }
            else if (line.substr(0, 3) == "vn ")
            {
                std::istringstream s(line.substr(3));
                vec3 n;
                s >> n.x;
                s >> n.y;
                s >> n.z;
                normals.push_back(n);
            }
            else if (line.substr(0, 2) == "f ")
            {
                if (line.find("//") == std::string::npos)
                {
                    std::istringstream s(line.substr(2));
                    face f;
                    s >> f.va;
                    s >> f.vb;
                    s >> f.vc;

                    if (f.va < 0 || f.vb < 0 || f.vc < 0)
                    {
                        f.va = vertices.size() + f.va;
                        f.vb = vertices.size() + f.vb;
                        f.vc = vertices.size() + f.vc;
                        f.va++;
                        f.vb++;
                        f.vc++;
                    }
                    else
                    {
                        f.va--;
                        f.vb--;
                        f.vc--;
                    }

                    f.material = str_currentMaterial;
                    indices.push_back(f);
                }
                else
                {
                    std::replace(line.begin(), line.end(), '/', ' ');
                    std::istringstream s(line.substr(2));
                    face f;
                    s >> f.va;
                    s >> f.na;
                    s >> f.vb;
                    s >> f.nb;
                    s >> f.vc;
                    s >> f.nc;

                    if (f.va < 0 || f.vb < 0 || f.vc < 0)
                    {
                        f.va = vertices.size() + f.va;
                        f.vb = vertices.size() + f.vb;
                        f.vc = vertices.size() + f.vc;
                        f.va++;
                        f.vb++;
                        f.vc++;
                    }
                    else
                    {
                        f.va--;
                        f.vb--;
                        f.vc--;
                    }

                    if (f.na < 0 || f.nb < 0 || f.nc < 0)
                    {
                        f.na = normals.size() + f.na;
                        f.nb = normals.size() + f.nb;
                        f.nc = normals.size() + f.nc;
                        f.na++;
                        f.nb++;
                        f.nc++;
                    }
                    else
                    {
                        f.na--;
                        f.nb--;
                        f.nc--;
                    }

                    f.material = str_currentMaterial;
                    indices.push_back(f);
                }
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        return 1;
    }

    for (size_t i = 0; i < indices.size(); i++)
    {
        std::array<Vertex, 3> verts;
        verts[0].setPosition(vertices[indices[i].va]);
        verts[1].setPosition(vertices[indices[i].vb]);
        verts[2].setPosition(vertices[indices[i].vc]);

        Material *mtl = m_material;

        if (!str_currentMaterial.empty() || !materials.empty())
        {
            mtl = materials.at(indices[i].material);
        }

        if (normals.size() > 0)
        {
            verts[0].setNormal(normals[indices[i].na]);
            verts[1].setNormal(normals[indices[i].nb]);
            verts[2].setNormal(normals[indices[i].nc]);
        }

        m_triangles.push_back(Triangle(m_objToWorld, m_worldToObj, mtl, verts));
    }

    return 0;
}

int Mesh::loadMTL(std::map<std::string, Material *> &materials)
{
    std::string str_currentMaterial;
    Material *mat_currentMaterial;

    std::ifstream file;
    std::string line;

    LOG("Attempting to load: " << m_mtlFileName);

    file.open("res/" + m_mtlFileName);

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            if (line.substr(0, 7) == "newmtl ")
            {
                str_currentMaterial = line.substr(7);
                mat_currentMaterial = m_matFactory->initDiffuseMaterial();
                //m_matFactory->initDiffuseMaterial(mat_currentMaterial);
                materials.insert(std::pair<std::string, Material *>(str_currentMaterial, mat_currentMaterial));
            }
            else if (line.substr(0, 3) == "Kd ") // Diffuse color
            {
                std::istringstream s(line.substr(3));
                vec3 kd;
                s >> kd.x;
                s >> kd.y;
                s >> kd.z;
                materials.at(str_currentMaterial)->setKd(kd);
            }
            else if (line.substr(0, 3) == "Ke ") // Emissive color (Blender doesn't want to export it, so it has to be added manually to the .mtl file)
            {
                std::istringstream s(line.substr(3));
                vec3 ke;
                s >> ke.x;
                s >> ke.y;
                s >> ke.z;
                materials.at(str_currentMaterial)->setKe(ke);
            }
            else if (line.substr(0, 3) == "Ni ") // Index of refraction
            {
                std::istringstream s(line.substr(3));
                float ior;
                s >> ior;
                // materials.at(str_currentMaterial).setIOR(ior); <-- No refractions yet :D
            }
            else if (line.substr(0, 6) == "illum ") // Illumination mode
            {
                std::istringstream s(line.substr(6));
                unsigned int illum;
                s >> illum;

                if (illum == 0 || illum == 1) // Diffuse surfaces
                {
                    //materials.at(str_currentMaterial).setReflT(DIFF);
                }
                if (illum == 4 || illum == 6 || illum == 7 || illum == 9) // Dielectric sufraces
                {
                    //materials.at(str_currentMaterial).setReflT(REFR);
                }
                else if (illum == 3) // Perfectly specular surfaces
                {
                    //materials.at(str_currentMaterial).setReflT(SPEC);
                }
                else if (illum == 5) // Glossy reflective surfaces
                {
                    //materials.at(str_currentMaterial).setReflT(GLOS);
                }
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

}
