#ifndef TEXTURE_H
#define TEXTURE_H

// std includes
#include <string>
#include <map>

// mirage includes
#include "../math/vec2.h"
#include "../math/vec3.h"

namespace mirage
{

	// Forward declarations
	class Texture;

	extern std::map<std::string, Texture *> LOADED_TEXTURES;

	class Texture
	{
	public:
		Texture(const std::string & filePath = "./res/textures/null.png");
		~Texture();
		vec3 sample(const vec2 & xy) const;
		void addReference();
		void delReference();
		uint32_t getRefAmount() const;
		unsigned char * getImage();
		int32_t getWidth() const;
		int32_t getHeight() const;
		int32_t getComponents() const;
		bool isHdr() const;
	private:
		std::string m_filePath;
		uint32_t m_refAmount;
		unsigned char * m_image;
		int32_t m_width;
		int32_t m_height;
		int32_t m_components;
		bool m_hdr;
	};

}


#endif // TEXTURE_H