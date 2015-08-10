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
    CameraOrtho cam_ortho(Transform(vec3(0, 0, -128)), film, 8, 64, 0.1f);
    CameraPersp cam_persp(Transform(vec3(0, 0.5f, -10)), film, 8, 64, 70.0f);
    Camera *camera = &cam_ortho;

    MatFactory matfactory;

    Transform lTransformW;
    Transform lTransformO;
    DiffuseMaterial matl1(vec3(), vec3(), vec3(1, 1, 1) * 10);
    DiffuseMaterial matl2(vec3(), vec3(), vec3(0.1f, 0.5f, 1) * 10);
    Sphere light(lTransformW, lTransformO, &matl1, vec3(8, -1, 4), 2.0f);
    Sphere lightb(lTransformW, lTransformO, &matl1, vec3(-8, 5, -4), 2.0f);

    DiffuseMaterial matdwhite(vec3(0.9f, 0.9f, 0.9f), vec3(), vec3());
    DiffuseMaterial matdgreen(vec3(0.1f, 0.9f, 0.1f), vec3(), vec3());
    DiffuseMaterial matdblue(vec3(0.1f, 0.1f, 0.9f), vec3(), vec3());
    SpecularMaterial matsmirror(vec3(1, 1, 1), vec3(), vec3());
    GlossyMaterial matglossy(vec3(0.87843137254f, 0.87450980392f, 0.85882352941f), vec3(), vec3(),  0.15f, 0.95f, 0.1f);
    GlossyMaterial matglossy2(vec3(0.80392156862f, 0.49803921568f, 0.19607843137f), vec3(), vec3(),  0.1f, 0.95f, 0.25f);
    Mesh dragon(Transform(vec3(0, -4, 0), quaternion().euler(0, 1, 0, 170), vec3(1, 1, 1)), Transform(), &matdblue, &matfactory, "dragon.obj");
    Mesh bunny1(Transform(vec3(-8, -4, -4), quaternion().euler(0, 1, 0, 170), vec3(5, 5, 5)), Transform(), &matglossy2, &matfactory, "bunny.obj");
    Mesh bunny2(Transform(vec3(8, -4, -6), quaternion().euler(0, 1, 0, 0), vec3(5, 5, 5)), Transform(), &matdgreen, &matfactory, "bunny.obj");
    Mesh plane(Transform(vec3(0, -4, 0), quaternion(), vec3(16, 16, 16)), Transform(), &matglossy, &matfactory, "plane.obj");
    Mesh cornellbox(Transform(vec3(0, 0, 0), quaternion().euler(0, 1, 0, 180), vec3(1, 1, 1)), Transform(), &matdwhite, &matfactory, "cornellbox_1.obj");

    std::vector<Shape *> shapes = dragon.getShapes();
    std::vector<Shape *> plane_shapes = plane.getShapes();
    std::vector<Shape *> bunny1_shapes = bunny1.getShapes();
    std::vector<Shape *> bunny2_shapes = bunny2.getShapes();

    for (size_t i = 0; i < bunny1_shapes.size(); i++)
    {
        shapes.push_back(bunny1_shapes[i]);
    }

    for (size_t i = 0; i < bunny2_shapes.size(); i++)
    {
        shapes.push_back(bunny2_shapes[i]);
    }

    for (size_t i = 0; i < plane_shapes.size(); i++)
    {
        shapes.push_back(plane_shapes[i]);
    }

    shapes.push_back(&light);
    shapes.push_back(&lightb);

    /*
    Mesh dragon(Transform(vec3(0, -4, 0), quaternion().euler(0, 1, 0, 170), vec3(1, 1, 1)), Transform(), &matdwhite, &matfactory, "dragon.obj");
    Mesh bunny(Transform(vec3(-8, -4, -4), quaternion().euler(0, 1, 0, 180), vec3(5, 5, 5)), Transform(), &matdred, &matfactory, "bunny.obj");
    Mesh plane(Transform(vec3(0, -4, 0), quaternion(), vec3(16, 16, 16)), Transform(), &matdwhite, &matfactory, "plane.obj");

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
    */

    KDTreeAccel accelerator(shapes);
    accelerator.init();
    Scene scene(&accelerator, camera);
    Pathtracer ptracer(vec3(0.75f, 0.87f, 0.98f) * 0.0f, 10.0f, 10);

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
