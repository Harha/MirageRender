// std includes
#include <fstream>

// mirage includes
#include "display.h"
#include "../math/mat4.h"

namespace mirage
{

	Display::Display(std::string title, unsigned width, unsigned height, unsigned scale) :
		m_title(title),
		m_width(width),
		m_height(height),
		m_scale(scale),
		m_isSavingImage(false),
		m_pixels(nullptr),
		m_window(NULL),
		m_renderer(NULL),
		m_texture(NULL)
	{
		init();

		LOG("Display: a New Display object has been created.");
		LOG("Display: Width: " << m_width << " Height: " << m_height << " Scale: " << m_scale);
	}

	Display::~Display()
	{
		DELETEA(m_pixels);

		SDL_DestroyTexture(m_texture);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	void Display::init()
	{
		m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width * m_scale, m_height * m_scale, SDL_WINDOW_SHOWN);
		if (m_window == nullptr)
		{
			ERR("SDL_CreateWindow Error: " << SDL_GetError());
			std::exit(1);
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
		if (m_renderer == nullptr)
		{
			SDL_DestroyWindow(m_window);
			ERR("SDL_CreateRenderer Error: " << SDL_GetError());
			std::exit(1);
		}

		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, m_width, m_height);
		if (m_texture == nullptr)
		{
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			ERR("SDL_CreateTexture Error: " << SDL_GetError());
			std::exit(1);
		}

		m_pixels = new(std::nothrow) int32_t[m_width * m_height];
		if (m_pixels == NULL)
		{
			SDL_DestroyTexture(m_texture);
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			ERR("m_pixels Memory allocation error! Program exit.");
			std::exit(1);
		}
	}

	void Display::setTitle(std::string title)
	{
		m_title = title;
		SDL_SetWindowTitle(m_window, m_title.c_str());
	}

	void Display::saveToPPM(std::string filePath)
	{
		std::ofstream file(filePath, std::ios::binary);

		std::string info_str = "#" + filePath + "\n";
		std::string size_str = std::to_string(m_width) + " " + std::to_string(m_height) + "\n";
		std::string data_str = "255\n";

		file.write("P3\n", 3);
		file.write(info_str.c_str(), info_str.size());
		file.write(size_str.c_str(), size_str.size());
		file.write(data_str.c_str(), data_str.size());

		std::string write_str = "";
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			write_str = std::to_string((m_pixels[i]) & 0xFF) + " " + std::to_string((m_pixels[i] >> 8) & 0xFF) + " " + std::to_string((m_pixels[i] >> 16) & 0xFF) + " ";
			file.write(write_str.c_str(), write_str.size());
		}

		file.close();

		LOG("Display: Saved a new screenshot to " << filePath);
	}

	unsigned Display::getWidth() const
	{
		return m_width;
	}

	unsigned Display::getHeight() const
	{
		return m_height;
	}

	unsigned Display::getScale() const
	{
		return m_scale;
	}

}
