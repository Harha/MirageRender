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
#include "core/objfactory.h"
#include "shapes/sphere.h"
#include "shapes/mesh.h"
#include "cameras/orthographic.h"
#include "cameras/perspective.h"
#include "accelerators/kdtree.h"
#include "renderers/pathtracer.h"
#include "materials/diffusemat.h"
#include "materials/specmat.h"
#include "materials/glassmat.h"
#include "materials/glossymat.h"
#include "lights/pointlight.h"
#include "lights/dirlight.h"
#include "lights/spotlight.h"
#include "core/luaengine.h"

using namespace mirage;

void dispose()
{
    SDL_Quit();

    LOG("Main: atexit(dispose) Hook called.");
}

int main(int argc, char **argv)
{
    // Print program name and version
    LOG("Main: MirageRender, version " << VERSION_R << "." << VERSION_B << "." << VERSION_A);

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ERR("Main: SDL_Init Error: " << SDL_GetError());
        return 1;
    }
    SDL_Event event;

    // Set-up atexit hooks
    atexit(dispose);

    // Initialize multithreading related stuff
#if THREADS>0
    const unsigned int tcount = THREADS;
#else
    const unsigned int tcount = SDL_GetCPUCount();
#endif
    std::thread *threads = new std::thread[tcount];

    LOG("Main: Initialized " << tcount << " worker threads...");

    // Initialize miragerender related variables/objects
    Display display("MirageRender", WIDTH, HEIGHT, SCALE);

    uint32_t startTime = SDL_GetTicks();
    uint32_t currentTime = SDL_GetTicks();
    uint32_t lastTime = 0;
    uint32_t frameCount = 0;
    float deltaTime = 0;
    float fps = 0;
    bool running = true;

    // Initialize Lua 5.3.x
    lua::init();

    // Run simple test lua script
    lua::load("res/scripts/test.lua");
    lua::exec("res/scripts/test.lua", "test");
    lua::exec("res/scripts/test.lua", "loop");

    // Load the scene
    Scene scene;

    // Pick camera
    Camera *camera = scene.getCamera();

    // Load the renderer
    Pathtracer renderer(vec3(1, 1, 1) * 0, scene.getRadClamping(), scene.getRecMax());

    // Main loop
    while (running)
    {
        // Calculate framerate related variables
        currentTime = SDL_GetTicks();
        deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        fps = frameCount / (static_cast<float>(SDL_GetTicks() - startTime) / 1000.0f);
        lastTime = currentTime;

        // Display info every n frames
        if (frameCount % 16 == 1)
        {
            std::string fps_str = std::to_string(fps);
            std::string dt_str = std::to_string(deltaTime);
            std::string title = "MirageRender | FPS: " + fps_str + " DT: " + dt_str;
            display.setTitle(title);
        }

        // Print info with F1
        if (g_keys[SDL_SCANCODE_F1])
        {
            LOG("cam " << camera->getTransform().getPosition().toString() << ", " << camera->getTransform().getOrientation().toString());
            LOG("spp: " << camera->getFilm().getSample(0, 0).getNumSamples());
        }

        // Save screenshot with F2
        if (g_keys[SDL_SCANCODE_F2]) // Image file saving
        {
            display.saveToPPM("render");
        }

        // Update camera / meshes
        camera->update(deltaTime, g_keys);

        // Render a portion of the screen per thread
        int width = camera->getFilm().getResolutionX();
        int height = camera->getFilm().getResolutionY();
        for (unsigned int i = 0; i < tcount; i++)
        {
            threads[i] = std::thread ([=,&renderer, &scene, &display]
            {
                renderer.render(&scene, &display, width, height/tcount, 0, height/tcount * i);
            });
        }

        // Wait for all the threads to finish on the main thread by joining them.
        for (unsigned int i = 0; i < tcount; i++)
        {
            threads[i].join();
        }

        // Display results on screen
        display.render();

        // Process SDL events
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

    // Unload lua 5.3.x
    lua::kill();

    // Clear all allocated heap memory
    delete[] threads;

    // Inform that we are done
    LOG("MirageRender, exit program successfully.");

    return 0;
}
