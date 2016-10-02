#include "texture.h"

// std includes

// lib includes
#define STB_IMAGE_IMPLEMENTATION
#include "../3rdparty/stb_image.h"

// mirage includes
#include "../macros.h"

namespace mirage
{

	std::map<std::string, Texture *> LOADED_TEXTURES;

	Texture::Texture(const std::string & filePath) :
		m_filePath(filePath),
		m_refAmount(1),
		m_image(NULL),
		m_width(0),
		m_height(0),
		m_components(0),
		m_hdr(false)
	{
		// Load a file only once, reuse the memory otherwise
		if (LOADED_TEXTURES.count(m_filePath) <= 0)
		{
			// Flip vertically on load
			stbi_set_flip_vertically_on_load(true);

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

			// Add this to loaded textures even if loading failed
			LOADED_TEXTURES[m_filePath] = this;
		}
		else
		{
			// Get the existing texture object
			Texture * loaded = LOADED_TEXTURES[m_filePath];

			// Copy data over to this class
			m_refAmount = loaded->getRefAmount();
			m_image = loaded->getImage();
			m_width = loaded->getWidth();
			m_height = loaded->getHeight();
			m_components = loaded->getComponents();
			m_hdr = loaded->isHdr();

			// Increase reference counts
			loaded->addReference();
			addReference();

			// Inform that the texture was reused
			LOG("Texture::Texture - Reusing already loaded (" << m_filePath << ", RefAmount: " << m_refAmount);
		}
	}

	Texture::~Texture()
	{
		// Get the loaded texture instance
		Texture * loaded = LOADED_TEXTURES[m_filePath];

		// Decrease reference count
		loaded->delReference();

		// Unload if no references left
		if (m_image != NULL && loaded->getRefAmount() <= 0)
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

	void Texture::addReference()
	{
		m_refAmount++;
	}

	void Texture::delReference()
	{
		m_refAmount = (m_refAmount <= 0) ? m_refAmount : m_refAmount - 1;
	}

	uint32_t Texture::getRefAmount() const
	{
		return m_refAmount;
	}

	unsigned char * Texture::getImage()
	{
		return m_image;
	}

	int32_t Texture::getWidth() const
	{
		return m_width;
	}

	int32_t Texture::getHeight() const
	{
		return m_height;
	}

	int32_t Texture::getComponents() const
	{
		return m_components;
	}

	bool Texture::isHdr() const
	{
		return m_hdr;
	}

}