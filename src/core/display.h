#ifndef DISPLAY_H
#define DISPLAY_H

// std includes
#include <iostream>
#include <string>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

class Display
{
public:
    Display(std::string title = "null", int width = 128, int height = 128, int scale = 4);
    ~Display();
    void render();
    void clear(const Uint32 &color);
    void setPixel(const int x, const int y, vec3 v);
    void setTitle(std::string title);
private:
    void init();
    std::string m_title;
    int m_width;
    int m_height;
    int m_scale;
    Uint32 *m_pixels;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;
};

}

#endif // DISPLAY_H
