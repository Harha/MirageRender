// std includes
#include <iostream>
#include <thread>
#include <string>
#include <vector>

// lib includes
#include "SDL2/SDL.h"

// mirage includes
#include "config.h"
#include "macros.h"
#include "core/input.h"
#include "core/display.h"
#include "core/scene.h"
#include "core/luaengine.h"
#include "renderers/pathtracer.h"

using namespace mirage;

void dispose()
{
    SDL_Quit();

    LOG("Main: atexit(dispose) Hook called.");
}

int main(int argc, char **argv)
{
    /* Print info and version */
    LOG("Main: MirageRender, version " << VERSION_R << "." << VERSION_B << "." << VERSION_A);

    /* Initialize SDL2 */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ERR("Main: SDL_Init Error: " << SDL_GetError());
        return 1;
    }
    SDL_Event event;

    /* Initialize function hooks */
    atexit(dispose);

    /* Initialize render threads */
#if THREADS>0
    const unsigned int tcount = THREADS;
#else
    const unsigned int tcount = SDL_GetCPUCount();
#endif
    std::thread *threads = new std::thread[tcount];
    LOG("Main: Initialized " << tcount << " rendering threads...");

    /* Initialize the main display */
    Display display("MirageRender", WIDTH, HEIGHT, SCALE);

    /* Initialize Scene & Lua 5.3.x and load script(s) */
    Scene scene;
    lua::init(&scene);
    lua::load("res/scripts/example.lua");

    /* Choose a camera and an accelerator */
    Camera *camera = scene.getCamera();
    Accelerator *accelerator = scene.getAccelerator();

    /* Initialize the chosen renderer */
    Pathtracer renderer(vec3(1, 1, 1) * 0, scene.getRadianceClamping(), scene.getMaxRecursion());

    /* FPS/DeltaTime related variables */
    uint32_t startTime = SDL_GetTicks();
    uint32_t currentTime = SDL_GetTicks();
    uint32_t lastTime = 0;
    uint32_t frameCount = 0;
    float deltaTime = 0;
    float fps = 0;
    bool running = true;

    while (running)
    {
        /* Calculate FPS and DeltaTime */
        currentTime = SDL_GetTicks();
        deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        fps = frameCount / (static_cast<float>(SDL_GetTicks() - startTime) / 1000.0f);
        lastTime = currentTime;

        /* Update title with info every 16th frame */
        if (frameCount % 16 == 1)
        {
            std::string fps_str = std::to_string(fps);
            std::string dt_str = std::to_string(deltaTime);
            std::string title = "MirageRender | FPS: " + fps_str + " DT: " + dt_str;
            display.setTitle(title);
        }

        /* Save screenshot to render.ppm with F2 */
        if (g_keys[SDL_SCANCODE_F2])
        {
            display.saveToPPM("render");
        }

        /* Render the scene if possible */
        if (camera && accelerator)
        {
            /* Update everything */
            camera->update(deltaTime, g_keys);

            /* Give a portion of the screen as a task for each thread */
            int width = camera->getFilm().getResolutionX();
            int height = camera->getFilm().getResolutionY();
            for (unsigned int i = 0; i < tcount; i++)
            {
                threads[i] = std::thread ([=,&renderer, &scene, &display]
                {
                    renderer.render(&scene, &display, width, height/tcount, 0, height/tcount * i);
                });
            }

            /* Wait for all the threads to finish on the main thread by joining them */
            for (unsigned int i = 0; i < tcount; i++)
            {
                threads[i].join();
            }

            /* Display results on screen */
            display.render();
        }
        /* Else, don't render anything and sleep for a while instead */
        /* This is just to keep the OS sane */
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        /* Process SDL events */
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
            {
                auto id = event.key.keysym.scancode;
                if (id > 0 && id < sizeof(g_keys) / sizeof(*g_keys))
                {
                    g_keys[id] = true;
                }
            }
            break;
            case SDL_KEYUP:
            {
                auto id = event.key.keysym.scancode;
                if (id > 0 && id < sizeof(g_keys) / sizeof(*g_keys))
                {
                    g_keys[id] = false;
                }
            }
            break;
            }
        }

        frameCount++;
    }

    /* Unload lua 5.3.x */
    lua::kill();

    /* Free the allocated rendering threads */
    delete[] threads;

    /* Inform that the program exit successfully */
    LOG("MirageRender, exit program successfully.");

    return 0;
}
