#include "mesh.h"

// std includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <functional>

// mirage includes
#include "../macros.h"
#include "../utils/strutils.h"
#include "../core/wavefrontfile.h"

namespace mirage
{

	Mesh::Mesh(const Transform o2w, Material *m, ObjFactory *objFactory, std::string fileName) : Shape(o2w, m), m_objFactory(objFactory)
	{
		if (!m_objFactory)
		{
			ERR("Mesh::Mesh - Supplied instance of ObjFactory is not loaded! Exiting program...");
			std::exit(1);
		}

		std::string test = filetoextension(fileName);

		// Wavefront .obj / .mtl model
		if (filetoextension(fileName) == "obj")
		{
			// Load the model into memory
			WavefrontFile file("./res/models/" + fileName);

			// Get the file data into temp references
			auto & points = file.getPoints();
			auto & normals = file.getNormals();
			auto & texcoords = file.getTexcoords();
			auto & meshes = file.getMeshes();
			auto & materials = file.getMaterials();

			// Extract the shapes from the model
			for (auto const & mesh : meshes)
			{
				// Hold loaded material info in this map
				std::map<std::string, Material *> loadedMaterials;

				for (auto const & face : mesh.second.faces)
				{
					// Create the vertices for our triangle
					std::array<Vertex, 3> verts;

					// Assign values
					if (!points.empty())
					{
						verts[0].setPosition(points[face.points[0]]);
						verts[1].setPosition(points[face.points[1]]);
						verts[2].setPosition(points[face.points[2]]);
					}
					else
					{
						ERR("Mesh::Mesh - WavefrontMaterial - Point vectors empty! Critical error converting .obj model to mesh.");
					}

					if (!normals.empty())
					{
						verts[0].setNormal(normals[face.normals[0]]);
						verts[1].setNormal(normals[face.normals[1]]);
						verts[2].setNormal(normals[face.normals[2]]);
					}
					else
					{
						ERR("Mesh::Mesh - WavefrontMaterial - Normal vectors empty! Critical error converting .obj model to mesh.");
					}

					if (!texcoords.empty())
					{
						verts[0].setTexcoord(texcoords[face.texcoords[0]]);
						verts[1].setTexcoord(texcoords[face.texcoords[1]]);
						verts[2].setTexcoord(texcoords[face.texcoords[2]]);
					}

					// Get current face material properties if they differ from previous and if the wf material exists
					if (loadedMaterials.count(face.material) <= 0 && materials.count(face.material) > 0)
					{
						WavefrontMaterial wf_material = materials.at(face.material);

						// Create appropriate material
						// TODO: Add missing material properties to WavefrontMaterial
						switch (wf_material.illum)
						{
						case 0:
						case 1:
						case 2:
							loadedMaterials[face.material] = m_objFactory->initDiffuseMaterial(wf_material.KdText, wf_material.Kd, wf_material.Ke);
							break;
						case 4:
						case 6:
						case 7:
						case 9:
							loadedMaterials[face.material] = m_objFactory->initDielectricMaterial(wf_material.Kd, wf_material.Ks, wf_material.Ke, wf_material.Ni);
							break;
						case 3:
						case 8:
							loadedMaterials[face.material] = m_objFactory->initSpecularMaterial(wf_material.Kd, wf_material.Ks, wf_material.Ke);
							break;
						case 5:
							loadedMaterials[face.material] = m_objFactory->initGlossyMaterial(
								wf_material.Kd,
								wf_material.Ks,
								wf_material.Ke,
								wf_material.Ns / 1000.0f,
								wf_material.Fr,
								1.0f - std::max(wf_material.Ks.x, std::max(wf_material.Ks.y, wf_material.Ks.z))
							);
							break;
						default:
							loadedMaterials[face.material] = m_objFactory->initDiffuseMaterial("", vec3(1, 1, 1), vec3(0, 0, 0));
							ERR("Mesh::Mesh - Loading of material " << face.material << " failed, no illumination mode was specified in the material properties file!");
							break;
						}
					}
					else if (loadedMaterials.count(face.material) <= 0 && materials.count(face.material) <= 0)
					{
						loadedMaterials[face.material] = m;
					}

					// Push our triangle to the vector
					m_triangles.push_back(Triangle(m_objToWorld, loadedMaterials[face.material], verts));
				}
			}
		}
		else
		{
			ERR("Mesh::Mesh - Error, unsupported file extension! Currently only wavefront .obj files are supported.");
			std::exit(1);
		}

		LOG("Mesh::Mesh - Loaded mesh successfully! Number of triangles: " << m_triangles.size());
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
		ERR("Called unimplemented method Mesh::intersect!");
		return false;
	}

	bool Mesh::intersectP(const Ray &ray) const
	{
		ERR("Called unimplemented method Mesh::intersectP!");
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

}
