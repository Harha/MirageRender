// std includes

// mirage includes
#include "display.h"
#include "../config.h"
#include "../macros.h"
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

	void Display::render()
	{
		SDL_UpdateTexture(m_texture, nullptr, m_pixels, m_width * sizeof(int32_t));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);
	}

	void Display::clear(const int32_t &color)
	{
		unsigned resolution = static_cast<unsigned>(m_width * m_height);
		for (size_t i = 0; i < resolution; i++)
			m_pixels[i] = color;
	}

	void Display::setPixel(const unsigned x, const unsigned y, vec3 v)
	{
		v = vec3::powv(v, 1.0f * GAMMA);
		v = vec3::clampv(v, 0.0f, 1.0f);

		auto r = static_cast<int32_t>(v.x * 255.0f);
		auto g = static_cast<int32_t>(v.y * 255.0f);
		auto b = static_cast<int32_t>(v.z * 255.0f);
		auto hex = ((r << 16) | (g << 8) | b);

		m_pixels[x + y * m_width] = hex;
	}

	void Display::setTitle(std::string title)
	{
		m_title = title;
		SDL_SetWindowTitle(m_window, m_title.c_str());
	}

	void Display::saveToPPM(std::string filename)
	{
		if (m_isSavingImage)
		{
			ERR("Display: Can't open a new file handle because an image is currently being saved, please wait...");
			return;
		}

		filename = filename + ".ppm";

		m_isSavingImage = true;
		FILE *f;

		if (fopen_s(&f, filename.c_str(), "w"))
		{
			ERR("Display: Writing image to a .ppm file failed... fopen returned NULL.");
			return;
		}

		fprintf(f, "P3\n%d %d\n%d\n", m_width, m_height, 255);
		for (size_t i = 0; i < m_width * m_height; i++)
		{
			fprintf(f, "%d %d %d ", (m_pixels[i] >> 16), (m_pixels[i] >> 8) & 0xFF, (m_pixels[i]) & 0xFF);
		}
		fclose(f);
		m_isSavingImage = false;

		LOG("Display: Screenshot successfully saved, filename: " << filename);
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
