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
    CameraOrtho cam_ortho(Transform(vec3(0, 0.5f, -128), quaternion().identity()), film, 8, 64, 0.01f);
    CameraPersp cam_persp(Transform(vec3(0, 1, 4.0f), quaternion().euler(0, 1, 0, 180)), film, 8, 64, 70.0f);
    Camera *camera = &cam_persp;
    ObjFactory objFactory;

    // Materials
    DiffuseMaterial matl1(vec3(), vec3(1, 1, 1) * 16);
    DiffuseMaterial matl2(vec3(), vec3(1.0f, 0.5f, 0.1f) * 16);

    DiffuseMaterial matdwhite(vec3(0.75f, 0.75f, 0.75f), vec3());
    DiffuseMaterial matdgreen(vec3(0.25f, 0.9f, 0.25f), vec3());
    DiffuseMaterial matdblue(vec3(0.25f, 0.25f, 0.9f), vec3());
    DiffuseMaterial matdyellow(vec3(0.75f, 0.75f, 0.25f), vec3());
    SpecularMaterial matmirror(vec3(1, 1, 1), vec3(), vec3());
    GlassMaterial matglass(vec3(1, 1, 1), vec3(), vec3(), 1.52f);
    GlassMaterial matglassr(vec3(0.9f, 0.25f, 0.25f), vec3(), vec3(), 1.52f);
    GlassMaterial matglassg(vec3(0.25f, 0.9f, 0.25f), vec3(), vec3(), 1.52f);
    GlassMaterial matglassy(vec3(0.9f, 0.9f, 0.25f), vec3(), vec3(), 1.52f);
    GlassMaterial matglassb(vec3(0.25f, 0.9f, 0.9f), vec3(), vec3(), 1.52f);
    GlossyMaterial matsilver(vec3(0.73f, 0.77f, 0.8f), vec3(), vec3(),  0.2f, 0.95f, 0.25f);
    GlossyMaterial matbronze(vec3(0.80392156862f, 0.49803921568f, 0.19607843137f), vec3(), vec3(),  0.05f, 0.95f, 0.1f);

    // Lightsources
    Sphere light(Transform(vec3(-2.5f, 5, -5)), &matl1, vec3(0, 0, 0), 1.0f);
    PointLight plight(Transform(vec3(0, 1.96f, 0)), vec3(0.9f, 1, 1) * 10.0f, 0, 0, 5);
    PointLight plight2(Transform(vec3(0, 1.96f, 0)), vec3(17, 12, 4) * 2.0f, 0, 0, 15);
    PointLight plight3(Transform(vec3(0, 4, 0)), vec3(1, 1, 1) * 20.0f, 0, 0, 0.05f);
    DirectionalLight dirlight(Transform(vec3(), quaternion(0.1f, -0.11f, -0.9f, -0.25f).normalize()), vec3(1, 1, 1) * 5.0f);

    // Meshes
    Mesh box(Transform(vec3(0, 0, 0), quaternion().euler(0, 1, 0, 0), vec3(1, 1, 1)), &matdwhite, &objFactory, "cornellbox_water.obj");
    //Mesh pla(Transform(vec3(0, 0, 0), quaternion().euler(0, 1, 0, 0), vec3(20, 1, 20)), &matdwhite, &objFactory, "plane.obj");
    //Mesh dragon(Transform(vec3(-0.3f, 1.19f, -0.1f), quaternion().euler(0, 1, 0, -33), vec3(0.05f, 0.05f, 0.05f)), &matsilver, &objFactory, "dragon.obj");

    Sphere sphere(Transform(vec3(-0.25f, 0.25f, 0.5f)), &matglassr, vec3(0, 0, 0), 0.25f);

    std::vector<Shape *> shapes = box.getShapes();
    /*std::vector<Shape *> shapes_pla = pla.getShapes();

    for (size_t i = 0; i < shapes_pla.size(); i++)
    {
        shapes.push_back(shapes_pla[i]);
    }*/

    shapes.push_back(&sphere);

    KDTreeAccel accelerator(shapes);
    accelerator.init();
    Scene scene(&accelerator, camera);
    //scene.addLight(&plight);
    //scene.addLight(&plight2);
    //scene.addLight(&plight3);
    //scene.addLight(&dirlight);
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

        if (g_keys[SDL_SCANCODE_F1]) // Print info
        {
            LOG("cam " << camera->getTransform().getPosition().toString() << ", " << camera->getTransform().getOrientation().toString());
            LOG("spp: " << camera->getFilm().getSample(0, 0).getNumSamples());
        }

        if (g_keys[SDL_SCANCODE_F2]) // Image file saving
        {
            display.saveToPPM("render");
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
