#ifndef WAVEFRONT_FILE_H
#define WAVEFRONT_FILE_H

// std includes
#include <string>
#include <vector>
#include <map>

// mirage includes
#include "../math/vec3.h"
#include "../math/vec2.h"

namespace mirage
{

	// ---------------------------------------------------------------------------
	// WavefrontFace
	// ---------------------------------------------------------------------------

	class WavefrontFace
	{
	public:
		std::vector<int> points;
		std::vector<int> normals;
		std::vector<int> texcoords;
		std::string material;

		WavefrontFace(const std::string & material = "NULL") :
			points(3),
			normals(3),
			texcoords(3),
			material(material)
		{

		}
	};

	// ---------------------------------------------------------------------------
	// WavefrontMaterial
	// ---------------------------------------------------------------------------

	class WavefrontMaterial
	{
	public:
		uint16_t illum; // Illumination mode
		vec3 Ka; // Ambient color
		vec3 Kd; // Diffuse color
		vec3 Ks; // Specular color
		vec3 Ke; // Emissive color

		WavefrontMaterial(const vec3 & Ka = vec3(), const vec3 & Kd = vec3(), const vec3 & Ks = vec3(), const vec3 & Ke = vec3()) :
			Ka(Ka),
			Kd(Kd),
			Ks(Ks),
			Ke(Ke)
		{

		}
	};

	// ---------------------------------------------------------------------------
	// WavefrontMesh
	// ---------------------------------------------------------------------------

	class WavefrontMesh
	{
	public:
		std::vector<WavefrontFace> faces;
		bool hasNormals;
		bool hasTexcoords;

		WavefrontMesh() :
			faces(0),
			hasNormals(false),
			hasTexcoords(false)
		{

		}
	};

	// ---------------------------------------------------------------------------
	// WavefrontFile
	// ---------------------------------------------------------------------------

	class WavefrontFile
	{
	public:
		WavefrontFile(const std::string & filePath = "NULL");
		void loadObj(const std::string & filePath);
		void loadMtl(const std::string & filePath);
		const std::vector<vec3> & getPoints() const;
		const std::vector<vec3> & getNormals() const;
		const std::vector<vec2> getTexcoords() const;
		const std::map<std::string, WavefrontMesh> & getMeshes() const;
		const std::map<std::string, WavefrontMaterial> & getMaterials() const;
	private:
		std::string m_objFilePath;
		std::string m_mtlFilePath;
		std::vector<vec3> m_points;
		std::vector<vec3> m_normals;
		std::vector<vec2> m_texcoords;
		std::map<std::string, WavefrontMesh> m_meshes;
		std::map<std::string, WavefrontMaterial> m_materials;
	};

}

#endif // WAVEFRONT_FILE_H