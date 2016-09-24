#include "texture.h"

// std includes

// lib includes
#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb_image.h"

// mirage includes
#include "../macros.h"

namespace mirage
{

	Texture::Texture(const std::string & filePath) :
		m_filePath(filePath),
		m_image(NULL),
		m_width(0),
		m_height(0),
		m_components(0),
		m_hdr(false)
	{
		// Attempt to load the image
		m_image = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_components, 4);

		// Handle success and error differently
		if (m_image != NULL)
		{
			LOG("Texture::Texture - " << m_filePath << " loaded successfully. W: " << m_width << ", H: " << m_height << ", N: " << m_components);
		}
		else
		{
			ERR("Texture::Texture - " << m_filePath << " failed to load!");
		}
	}

	Texture::~Texture()
	{
		if (m_image != NULL)
		{
			stbi_image_free(m_image);

			LOG("Texture::~Texture - Unloaded texture successfully.");
		}
	}

	vec3 Texture::sample(const vec2 & xy) const
	{
		// Get x and y, scale them from 0-1 to 0-size
		int32_t x = static_cast<int32_t>(std::abs(xy.x * m_width));
		int32_t y = static_cast<int32_t>(std::abs(xy.y * m_height));

		// Get RGB values, leave A alone for now...
		int32_t index = x + y * m_width;
		int32_t r = m_image[(index * 4 + 0) % (m_width * m_height * 4)];
		int32_t g = m_image[(index * 4 + 1) % (m_width * m_height * 4)];
		int32_t b = m_image[(index * 4 + 2) % (m_width * m_height * 4)];

		// Return the color as float components normalized to 0.0 - 1.0 range
		static const float DIVISOR = 1.0f / 255.0f;
		return vec3(static_cast<float>(r * DIVISOR), static_cast<float>(g * DIVISOR), static_cast<float>(b * DIVISOR));
	}

}