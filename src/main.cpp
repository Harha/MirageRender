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
#include "renderers/raytracer.h"
#include "materials/diffusemat.h"
#include "materials/specmat.h"
#include "materials/glassmat.h"
#include "materials/glossymat.h"
#include "core/matfactory.h"

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

    // Initialize test scene data / seed scene, build acc, etc...
    Film film(WIDTH, HEIGHT);
    CameraOrtho cam_ortho(Transform(vec3(0, -4, -128), quaternion(0.88f, 0.31f, -0.35f, 0.06f)), film, 8, 64, 0.075f);
    CameraPersp cam_persp(Transform(vec3(0, 0, -22.0f)), film, 8, 64, 70.0f);
    Camera *camera = &cam_persp;

    MatFactory matfactory;

    // Materials
    DiffuseMaterial matl1(vec3(), vec3(1, 1, 1) * 16);
    DiffuseMaterial matl2(vec3(), vec3(1.0f, 0.5f, 0.1f) * 16);

    DiffuseMaterial matdwhite(vec3(0.9f, 0.9f, 0.9f), vec3());
    DiffuseMaterial matdgreen(vec3(0.25f, 0.75f, 0.25f), vec3());
    DiffuseMaterial matdblue(vec3(0.1f, 0.1f, 0.9f), vec3());
    SpecularMaterial matmirror(vec3(1, 1, 1), vec3(), vec3());
    GlassMaterial matglass(vec3(1, 1, 1), vec3(), vec3(), 1.52f);
    GlossyMaterial matsilver(vec3(0.87843137254f, 0.87450980392f, 0.85882352941f), vec3(), vec3(),  0.1f, 0.95f, 0.25f);
    GlossyMaterial matbronze(vec3(0.80392156862f, 0.49803921568f, 0.19607843137f), vec3(), vec3(),  0.1f, 0.95f, 0.25f);

    // Lightsources
    Sphere light(Transform(vec3(0, 10, 5)), &matl1, vec3(0, 0, 0), 2.0f);

    // Meshes
    Mesh box(Transform(vec3(0, 0, 0), quaternion().euler(0, 1, 0, 0), vec3(5, 5, 5)), &matdwhite, &matfactory, "cornellbox_empty.obj");
    Mesh dragon(Transform(vec3(2.0f, -5, -2.5f), quaternion().euler(0, 1, 0, 20), vec3(0.5f, 0.5f, 0.5f)), &matdgreen, &matfactory, "dragon.obj");
    Mesh bunny(Transform(vec3(-2.5f, -4.75f, 1.5f), quaternion().euler(0, 1, 0, 170), vec3(2, 2, 2)), &matsilver, &matfactory, "bunny.obj");

    std::vector<Shape *> shapes = box.getShapes();
    std::vector<Shape *> shapes_dragon = dragon.getShapes();
    std::vector<Shape *> shapes_bunny = bunny.getShapes();

    for (size_t i = 0; i < shapes_dragon.size(); i++)
    {
        shapes.push_back(shapes_dragon[i]);
    }

    for (size_t i = 0; i < shapes_bunny.size(); i++)
    {
        shapes.push_back(shapes_bunny[i]);
    }

    //shapes.push_back(&light);

    KDTreeAccel accelerator(shapes);
    accelerator.init();
    Scene scene(&accelerator, camera);
    Pathtracer ptracer(vec3(0.75f, 0.87f, 0.98f) * 0.0f, 10.0f, 5);

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

        if (g_keys[SDL_SCANCODE_SPACE])
        {
            LOG("cam pos: " << camera->getTransform().getPosition().toString() << " rot: " << camera->getTransform().getOrientation().toString());
        }

        // Render a portion of the screen per thread
        int width = camera->getFilm().getResolutionX();
        int height = camera->getFilm().getResolutionY();
        for (unsigned int i = 0; i < tcount; i++)
        {
            threads[i] = std::thread ([=,&ptracer, &scene, &display]
            {
                ptracer.render(&scene, &display, width, height/tcount, 0, height/tcount * i);
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

    delete[] threads;
    LOG("MirageRender, exit program successfully.");

    return 0;
}
