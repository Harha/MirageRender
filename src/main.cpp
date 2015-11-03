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

    // Camera film
    Film film(WIDTH, HEIGHT);

    // Cameras to be used
    CameraOrtho cam_ortho(Transform(vec3(0, 0, -128), quaternion().identity()), film, 8, 64, 0.01f);
    CameraPersp cam_persp(Transform(vec3(0.25f, 2, 3.0f), quaternion(-0.1f, 0.0f, 0.98f, -0.15f).normalize()), film, 8, 64, 70.0f);

    // Chosen camera
    Camera *camera = &cam_persp;

    // Object factory
    ObjFactory obj_factory;

    // Materials
    DiffuseMaterial diffuse_white(vec3(0.75f, 0.75f, 0.75f), vec3());
    DiffuseMaterial diffuse_cyan(vec3(0.25f, 0.75f, 0.75f), vec3());
    SpecularMaterial spec_mirror(vec3(1, 1, 1));
    GlossyMaterial glossy_gold(vec3(1.0f, 0.9f, 0.4f), vec3(), vec3(), 0.05f, 0.75f, 0.1f);

    // Meshes
    Mesh mesh_main(Transform(vec3(0, 0, 0), quaternion().identity(), vec3(1, 1, 1) * 5.0f), &diffuse_white, &obj_factory, "plane.obj");
    Mesh mesh_objt_d(Transform(vec3(-1.25f, 0, -1), quaternion().identity(), vec3(1, 1, 1) * 0.5f), &diffuse_white, &obj_factory, "mitsuba_sphere.obj");
    Mesh mesh_objt_g(Transform(vec3(0, 0, -2), quaternion().identity(), vec3(1, 1, 1) * 0.5f), &glossy_gold, &obj_factory, "mitsuba_sphere.obj");
    Mesh mesh_dirl(Transform(vec3(-2.25f, 0.1f, -1.2f), quaternion().euler(1, 0, 0, 90.0f) * quaternion().euler(0, 1, 0, -70.0f), vec3(1, 1, 1) * 0.1f), &diffuse_white, &obj_factory, "directional_light.obj");

    // Allocate memory for all the shapes to be stored in a vector
    std::vector<Shape *> shapes_main = mesh_main.getShapes();
    std::vector<Shape *> shapes_objt_d = mesh_objt_d.getShapes();
    std::vector<Shape *> shapes_objt_g = mesh_objt_g.getShapes();
    std::vector<Shape *> shapes_dirl = mesh_dirl.getShapes();
    std::vector<Shape *> shapes_allo;

    // Insert shapes from meshes to the vector
    for (size_t i = 0; i < shapes_main.size(); i++)
    {
        shapes_allo.push_back(shapes_main[i]);
    }

    for (size_t i = 0; i < shapes_objt_d.size(); i++)
    {
        shapes_allo.push_back(shapes_objt_d[i]);
    }

    for (size_t i = 0; i < shapes_objt_g.size(); i++)
    {
        shapes_allo.push_back(shapes_objt_g[i]);
    }

    for (size_t i = 0; i < shapes_dirl.size(); i++)
    {
        shapes_allo.push_back(shapes_dirl[i]);
    }

    // Lights
    PointLight plight_1(Transform(vec3(0, 2.5f, 0)), vec3(1, 0.9f, 0.75f) * 15.0f, 0, 0, 1);
    //PointLight plight_2(Transform(vec3(-0.24f, 0.1f, -1.105f)), vec3(1, 1, 1) * 15.0f, 0, 0, 5);
    SpotLight slight_1(Transform(vec3(0, 0.5f, 0.0f), quaternion().euler(0, 1, 0, 25)), vec3(1, 0.9f, 0.75f) * 10.0f, 0, 0, 1, 0.6f);

    // Ray acceleration tree structure
    KDTreeAccel accelerator(shapes_allo);
    accelerator.init();

    // Scene object
    Scene scene(&accelerator, camera);
    //scene.addLight(&plight_2);
    scene.addLight(&slight_1);

    // Renderer object
    Pathtracer renderer(vec3(1, 1, 1) * 0.25f, 10.0f, 5);

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

    // Clear all allocated heap memory
    delete[] threads;

    // Inform that we are done
    LOG("MirageRender, exit program successfully.");

    return 0;
}
