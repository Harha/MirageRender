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
#include "core/ray.h"
#include "core/accelerator.h"
#include "core/shape.h"
#include "shapes/sphere.h"
#include "cameras/orthographic.h"
#include "cameras/perspective.h"
#include "accelerators/kdtree.h"
#include "core/vertex.h"
#include "shapes/mesh.h"
#include "core/scene.h"
#include "renderers/pathtracer.h"

using namespace mirage;

void dispose()
{
    SDL_Quit();

    LOG("atexit(dispose) Hook called.");
}

int main(int argc, char **argv)
{
    // Print program name and version
    LOG("MirageRender, version " << VERSION_R << "." << VERSION_B << "." << VERSION_A);

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ERR("SDL_Init Error: " << SDL_GetError());
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

    // Initialize miragerender related variables/objects
    Display display("MirageRender", WIDTH, HEIGHT, SCALE);

    uint32_t startTime = SDL_GetTicks();
    uint32_t currentTime = SDL_GetTicks();
    uint32_t lastTime = 0;
    uint32_t frameCount = 0;
    float deltaTime = 0;
    float fps = 0;
    bool running = true;

    // Main film
    Film film(WIDTH, HEIGHT);

    // 2 Main cameras
    CameraOrtho cam_ortho(Transform(vec3(0, -1, -128)), film, 8, 64, 0.05f);
    CameraPersp cam_persp(Transform(vec3(0, 0.5f, -10)), film, 8, 64, 70.0f);

    // Main camera pointer
    Camera *camera = &cam_ortho;

    // Create 1 area light source
    Transform lTransformW;
    Transform lTransformO;
    Sphere light(lTransformW, lTransformO, Material(vec3(), vec3(), vec3(1, 1, 1) * 10), vec3(8, -2, -4), 2.0f);
    Sphere lightb(lTransformW, lTransformO, Material(vec3(), vec3(), vec3(0, 0.25f, 1) * 10), vec3(-8, 5, -4), 2.0f);

    // Create 1 mesh to render
    //Mesh sponza(Transform(), Transform(), Material(vec3(1, 1, 1)), "sponza.obj");
    Mesh dragon(Transform(vec3(0, -4, 0), quaternion().euler(0, 1, 0, 170), vec3(1, 1, 1)), Transform(), Material(vec3(1, 1, 1)), "dragon.obj");
    Mesh bunny(Transform(vec3(-8, -4, -4), quaternion().euler(0, 1, 0, 170), vec3(5, 5, 5)), Transform(), Material(vec3(0.9f, 0.1f, 0.1f)), "bunny.obj");
    Mesh plane(Transform(vec3(0, -4, 0), quaternion(), vec3(16, 16, 16)), Transform(), Material(vec3(1, 1, 1)), "plane.obj");

    std::vector<Shape *> shapes = dragon.getShapes();
    std::vector<Shape *> plane_shapes = plane.getShapes();
    std::vector<Shape *> bunny_shapes = bunny.getShapes();

    for (size_t i = 0; i < bunny_shapes.size(); i++)
    {
        shapes.push_back(bunny_shapes[i]);
    }

    for (size_t i = 0; i < plane_shapes.size(); i++)
    {
        shapes.push_back(plane_shapes[i]);
    }

    shapes.push_back(&light);
    shapes.push_back(&lightb);

    // Create 1 acceleration structure
    KDTreeAccel accelerator(shapes);

    // Build the tree
    accelerator.init();

    // Initialize a new scene
    Scene scene(&accelerator, camera);

    // Initialize pathtracer renderer
    Pathtracer ptracer(5);

    while (running)
    {
        // Calculate framerate related variables
        currentTime = SDL_GetTicks();
        deltaTime = static_cast<float>(currentTime - lastTime) / 1000.0f;
        fps = frameCount / (static_cast<float>(SDL_GetTicks() - startTime) / 1000.0f);
        lastTime = currentTime;

        // Display info
        if (frameCount % 16 == 1)
        {
            std::string fps_str = std::to_string(fps);
            std::string dt_str = std::to_string(deltaTime);
            std::string title = "MirageRender | FPS: " + fps_str + " DT: " + dt_str;
            display.setTitle(title);
        }

        // Update everything
        camera->update(deltaTime, g_keys);

        // Render everything
        ptracer.render(&scene, &display);
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

    delete[] threads;
    LOG("MirageRender, exit program successfully.");

    return 0;
}
