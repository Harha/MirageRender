#ifndef TEXTURE_H
#define TEXTURE_H

// std includes
#include <string>

// mirage includes
#include "../math/vec2.h"
#include "../math/vec3.h"

namespace mirage
{

	class Texture
	{
	public:
		Texture(const std::string & filePath = "./res/textures/null.png");
		~Texture();
		vec3 sample(const vec2 & xy) const;
	private:
		std::string m_filePath;
		unsigned char * m_image;
		int32_t m_width;
		int32_t m_height;
		int32_t m_components;
		bool m_hdr;
	};

}


#endif // TEXTURE_H