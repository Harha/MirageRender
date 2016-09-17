// std includes
#include <iostream>
#include <thread>
#include <string>
#include <vector>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "config.h"
#include "macros.h"
#include "utils/strutils.h"
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

int main(int argc, char * argv[])
{
	// Print info and version
	LOG("Main: MirageRender, version " << VERSION_R << "." << VERSION_B << "." << VERSION_A);

	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ERR("Main: SDL_Init Error: " << SDL_GetError());
		return 1;
	}
	SDL_Event event;

	// Initialize function hooks
	atexit(dispose);

	// Parse launch parameters, get script, etc objects..
	std::string script("example.lua");
	switch (cstr2int(argc > 1 ? argv[1] : "default"))
	{
	case cstr2int("--script"):
	case cstr2int("-s"):
		if (argc > 2)
			script = argv[2];
		else
			LOG("No script file specified. Loading default script (" << script << ").");
		break;
	case cstr2int("--help"):
	case cstr2int("-h"):
		LOG("Usage: mirage.exe --script scriptfilename.lua, folder for scripts is ./res/scripts/\n"
			<< "      Available launch parameters:\n"
			<< "      Show this help message: --help, -h\n"
			<< "      Load a scene file: --script, -s"
		);
		return 0;
	default:
		LOG("No script file specified. Loading default script (" << script << ").");
		break;
	}

	// Initialize Scene & Lua 5.3.x + load script(s)
	Scene scene;
	lua::init(&scene);
	lua::load(std::string("./res/scripts/") + script);

	// Initialize render threads
	const unsigned tcount = lua::g_mThreadInitInfo.rThreadCount;
	std::thread *threads = new std::thread[tcount];
	LOG("Main: Initialized " << tcount << " rendering thread(s)...");

	// Initialize the main display
	Display display("MirageRender",
		lua::g_dispInitInfo.width,
		lua::g_dispInitInfo.height,
		lua::g_dispInitInfo.scale);

	// Choose a camera and an accelerator
	Camera * camera = scene.getCamera();
	Accelerator * accelerator = scene.getAccelerator();

	// Initialize the chosen renderer
	Pathtracer renderer(vec3(1, 1, 1) * 0, scene.getRadianceClamping(), scene.getMaxRecursion());

	// Runtime related state variables
	uint32_t frameCount = 0;
	double frameDelta = 0.0;
	bool running = true;

	while (running)
	{
		// Get frame start time
		uint32_t t_start = SDL_GetTicks();

		// F1, print information about the rendering process
		if (g_keys[SDL_SCANCODE_F1])
		{
			LOG("Frames: " << frameCount << ", Delta: " << frameDelta << "ms, Samples per pixel: " << camera->getFilm().getSample(0, 0).getNumSamples());
			LOG("Camera pos: " << camera->getTransform().getPosition().toString());
			LOG("Camera ori: " << camera->getTransform().getOrientation().toString());
		}

		// F2, save the current framebuffer to a .ppm image file
		if (g_keys[SDL_SCANCODE_F2])
		{
			display.saveToPPM("render");
		}

		// Render the scene if possible
		if (camera && accelerator)
		{
			// Update everything
			camera->update(0.025, g_keys);

			// Give a portion of the screen as a task for each thread
			int width = camera->getFilm().getResolutionX();
			int height = camera->getFilm().getResolutionY();
			for (unsigned int i = 0; i < tcount; i++)
			{
				threads[i] = std::thread([=, &renderer, &scene, &display]
				{
					renderer.render(&scene, &display, width, height / tcount, 0, height / tcount * i);
				});
			}

			// Wait for all the threads to finish on the main thread by joining them
			for (unsigned int i = 0; i < tcount; i++)
			{
				threads[i].join();
			}

			// Display results on screen
			display.render();

			// Increment framecount
			if (camera->getTransform().reqStateUpdate() == false)
			{
				frameCount++;
			}
			else
			{
				camera->getTransform().setState(false);
				frameCount = 0;
			}
		}
		// Else, don't render anything and sleep for a while instead
		// This is just to keep the OS sane by letting CPU sleep
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

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

		// Get frame deltaTime
		frameDelta = static_cast<double>(SDL_GetTicks() - t_start);
	}

	// Unload lua 5.3.x
	lua::kill();

	// Free the allocated rendering threads
	DELETEA(threads);

	// Inform that the program exit successfully
	LOG("MirageRender, exit program successfully.");

	return 0;
}
