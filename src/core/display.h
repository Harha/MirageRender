#ifndef DISPLAY_H
#define DISPLAY_H

// std includes
#include <iostream>
#include <string>
#include <cstdio>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "../macros.h"
#include "../math/vec3.h"

namespace mirage
{

	class Display
	{
	public:
		Display(std::string title = "null", unsigned width = 128, unsigned height = 128, unsigned scale = 4);
		~Display();

		inline void render()
		{
			SDL_UpdateTexture(m_texture, nullptr, m_pixels, m_width * sizeof(int32_t));
			SDL_RenderClear(m_renderer);
			SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
			SDL_RenderPresent(m_renderer);
		}

		inline void clear(const int32_t & color)
		{
			unsigned resolution = static_cast<unsigned>(m_width * m_height);
			for (size_t i = 0; i < resolution; i++)
				m_pixels[i] = color;
		}

		inline void setPixel(const unsigned x, const unsigned y, vec3 v)
		{
			v = vec3::powv(v, 1.0f * GAMMA);
			v = vec3::clampv(v, 0.0f, 1.0f);

			auto r = static_cast<int32_t>(v.x * 255.0f);
			auto g = static_cast<int32_t>(v.y * 255.0f);
			auto b = static_cast<int32_t>(v.z * 255.0f);
			auto hex = ((r << 16) | (g << 8) | b);

			m_pixels[x + y * m_width] = hex;
		}

		void setTitle(std::string title);
		void saveToPPM(std::string filePath);
		unsigned getWidth() const;
		unsigned getHeight() const;
		unsigned getScale() const;
	private:
		void init();
		std::string m_title;
		unsigned m_width;
		unsigned m_height;
		unsigned m_scale;
		bool m_isSavingImage;
		int32_t *m_pixels;
		SDL_Window *m_window;
		SDL_Renderer *m_renderer;
		SDL_Texture *m_texture;
	};

}

#endif // DISPLAY_H
