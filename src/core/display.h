#ifndef DISPLAY_H
#define DISPLAY_H

// std includes
#include <iostream>
#include <string>
#include <cstdio>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

	class Display
	{
	public:
		Display(std::string title = "null", unsigned width = 128, unsigned height = 128, unsigned scale = 4);
		~Display();
		void render();
		void clear(const int32_t &color);
		void setPixel(const unsigned x, const unsigned y, vec3 v);
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
