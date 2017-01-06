// std includes
#include <iostream>
#include <cstring>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "luaengine.h"
#include "../macros.h"
#include "../shapes/mesh.h"
#include "../shapes/sphere.h"
#include "accelerator.h"
#include "../accelerators/bvh.h"

namespace mirage
{

	namespace lua
	{

		lua_State * g_state;
		Scene * g_scene;
		DisplayInitInfo g_dispInitInfo;
		MultiThreadInitInfo g_mThreadInitInfo;

		void init(Scene * scene)
		{
			MLOG_INFO("Lua: Version: %s", LUA_COPYRIGHT);

			if (!g_state)
			{
				g_state = luaL_newstate();
				g_scene = scene;
				g_dispInitInfo = { 512, 512, 1 };
				g_mThreadInitInfo = { static_cast<unsigned>(SDL_GetCPUCount()) };

				luaopen_io(g_state);
				luaopen_base(g_state);
				luaopen_table(g_state);
				luaopen_string(g_state);
				luaopen_math(g_state);
				luaL_openlibs(g_state);

				MLOG_INFO("Lua: Scripting engine initialized.");
			}
			else
			{
				MLOG_ERROR("Lua: There was a problem initializing the scripting engine.");
				std::exit(1);
			}
		}

		void kill()
		{
			if (g_state)
			{
				lua_close(g_state);

				MLOG_INFO("Lua: Sripting engine unloaded successfully.");
			}
			else
			{
				MLOG_ERROR("Lua: There was a problem unloading the scripting engine.");
				std::exit(1);
			}
		}

		void rerr(int status)
		{
			if (g_state)
			{
				if (status != 0)
				{
					MLOG_ERROR("Lua: %s", lua_tostring(g_state, -1));
					lua_pop(g_state, 1);
				}
			}
		}

		void load(const std::string & filename)
		{
			if (g_state)
			{
				// Load the lua script into memory
				int status = luaL_loadfile(g_state, filename.c_str());

				// Register available C/C++ functions & objects

				// Utility functions 
				lua_pushcfunction(g_state, lua_print_func);
				lua_setglobal(g_state, "print");

				// C++ Object constructors
				lua_pushcfunction(g_state, lua_NewVector3_func);
				lua_setglobal(g_state, "NewVector3");
				lua_pushcfunction(g_state, lua_NewVector4_func);
				lua_setglobal(g_state, "NewVector4");
				lua_pushcfunction(g_state, lua_NewQuaternion_func);
				lua_setglobal(g_state, "NewQuaternion");
				lua_pushcfunction(g_state, lua_NewQuaternionLookAt_func);
				lua_setglobal(g_state, "NewQuaternionLookAt");
				lua_pushcfunction(g_state, lua_NewTransform_func);
				lua_setglobal(g_state, "NewTransform");
				lua_pushcfunction(g_state, lua_NewCameraOrtho_func);
				lua_setglobal(g_state, "NewCameraOrtho");
				lua_pushcfunction(g_state, lua_NewCameraPersp_func);
				lua_setglobal(g_state, "NewCameraPersp");
				lua_pushcfunction(g_state, lua_NewLightDir_func);
				lua_setglobal(g_state, "NewLightDir");
				lua_pushcfunction(g_state, lua_NewLightPoint_func);
				lua_setglobal(g_state, "NewLightPoint");
				lua_pushcfunction(g_state, lua_NewLightSpot_func);
				lua_setglobal(g_state, "NewLightSpot");
				lua_pushcfunction(g_state, lua_NewMesh_func);
				lua_setglobal(g_state, "NewMesh");
				lua_pushcfunction(g_state, lua_NewSphere_func);
				lua_setglobal(g_state, "NewSphere");
				lua_pushcfunction(g_state, lua_NewDiffMaterial_func);
				lua_setglobal(g_state, "NewDiffMaterial");
				lua_pushcfunction(g_state, lua_NewEmisMaterial_func);
				lua_setglobal(g_state, "NewEmisMaterial");
				lua_pushcfunction(g_state, lua_NewDielectricMaterial_func);
				lua_setglobal(g_state, "NewDielectricMaterial");
				lua_pushcfunction(g_state, lua_NewSpecMaterial_func);
				lua_setglobal(g_state, "NewSpecMaterial");
				lua_pushcfunction(g_state, lua_NewGlossyMaterial_func);
				lua_setglobal(g_state, "NewGlossyMaterial");

				// C++ Scene object adders
				lua_pushcfunction(g_state, lua_AddCamera_func);
				lua_setglobal(g_state, "AddCamera");
				lua_pushcfunction(g_state, lua_AddLight_func);
				lua_setglobal(g_state, "AddLight");
				lua_pushcfunction(g_state, lua_AddMesh_func);
				lua_setglobal(g_state, "AddMesh");
				lua_pushcfunction(g_state, lua_AddShape_func);
				lua_setglobal(g_state, "AddShape");
				lua_pushcfunction(g_state, lua_AddRayAccelerator_func);
				lua_setglobal(g_state, "AddRayAccelerator");

				// C++ Scene setting setters
				lua_pushcfunction(g_state, lua_SetDisplayInitInfo_func);
				lua_setglobal(g_state, "SetDisplayInitInfo");
				lua_pushcfunction(g_state, lua_SetMThreadInitInfo_func);
				lua_setglobal(g_state, "SetMThreadInitInfo");
				lua_pushcfunction(g_state, lua_SetRadianceClamping_func);
				lua_setglobal(g_state, "SetRadianceClamping");
				lua_pushcfunction(g_state, lua_SetMaxRecursion_func);
				lua_setglobal(g_state, "SetMaxRecursion");
				lua_pushcfunction(g_state, lua_SetSceneSkyColor_func);
				lua_setglobal(g_state, "SetSceneSkyColor");

				// Execute the program if no errors found
				if (status == 0)
				{
					// Initially run the script once
					lua_call(g_state, 0, 0);

					// Execute the init function
					exec(filename, "init");
				}

				// Report found errors
				rerr(status);
			}
		}

		void exec(const std::string & filename, const std::string & funcname)
		{
			// Get the desired function from the environment
			lua_getglobal(g_state, funcname.c_str());

			// Call the desired function
			lua_call(g_state, 0, 0);
		}

		// Utility functions
		int lua_print_func(lua_State * L)
		{
			// Get the function argument
			const char *str = luaL_checkstring(L, 1);

			// Call the underlying C++ function
			LOG("LuaScript: " << str);

			return 0;
		}

		// C++ Object constructors
		int lua_NewVector3_func(lua_State * L)
		{
			// Get the function arguments
			float x = luaL_checknumber(L, 1);
			float y = luaL_checknumber(L, 2);
			float z = luaL_checknumber(L, 3);

			// Create the object and return the address
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initVector3(x, y, z));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewVector4_func(lua_State * L)
		{
			// Get the function arguments
			float x = luaL_checknumber(L, 1);
			float y = luaL_checknumber(L, 2);
			float z = luaL_checknumber(L, 3);
			float w = luaL_checknumber(L, 4);

			// Create the object and return the address
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initVector4(x, y, z, w));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewQuaternion_func(lua_State * L)
		{
			// Get the function arguments 
			float w = luaL_checknumber(L, 1);
			float x = luaL_checknumber(L, 2);
			float y = luaL_checknumber(L, 3);
			float z = luaL_checknumber(L, 4);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initQuaternion(w, x, y, z));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewQuaternionLookAt_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_origin_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_target_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 2));

			// Get the objects from address 
			vec3 *origin_vec3 = reinterpret_cast<vec3 *>(address_origin_vec3);
			vec3 *target_vec3 = reinterpret_cast<vec3 *>(address_target_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initQuaternion(*origin_vec3, *target_vec3));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewTransform_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_pos_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_ori_quat = static_cast<std::size_t>(luaL_checkinteger(L, 2));
			std::size_t address_scl_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 3));

			// Get the objects from address 
			vec3 *pos_vec3 = reinterpret_cast<vec3 *>(address_pos_vec3);
			quaternion *ori_quat = reinterpret_cast<quaternion *>(address_ori_quat);
			vec3 *scl_vec3 = reinterpret_cast<vec3 *>(address_scl_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initTransform(*pos_vec3, *ori_quat, *scl_vec3));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewCameraOrtho_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			float speed = luaL_checknumber(L, 2);
			float sensitivity = luaL_checknumber(L, 3);
			float zoom = luaL_checknumber(L, 4);

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initOrthoCamera(*t, speed, sensitivity, zoom,
				g_dispInitInfo.width,
				g_dispInitInfo.height));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewCameraPersp_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			float speed = luaL_checknumber(L, 2);
			float sensitivity = luaL_checknumber(L, 3);
			float fov = luaL_checknumber(L, 4);

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initPerspCamera(*t, speed, sensitivity, fov,
				g_dispInitInfo.width,
				g_dispInitInfo.height));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewLightDir_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_emission = static_cast<std::size_t>(luaL_checkinteger(L, 2));

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);
			vec3 *e = reinterpret_cast<vec3 *>(address_emission);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDirLight(*t, *e));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewLightPoint_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_emission = static_cast<std::size_t>(luaL_checkinteger(L, 2));
			float aC = luaL_checknumber(L, 3);
			float aL = luaL_checknumber(L, 4);
			float aQ = luaL_checknumber(L, 5);

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);
			vec3 *e = reinterpret_cast<vec3 *>(address_emission);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initPointLight(*t, *e, aC, aL, aQ));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewLightSpot_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_emission = static_cast<std::size_t>(luaL_checkinteger(L, 2));
			float aC = luaL_checknumber(L, 3);
			float aL = luaL_checknumber(L, 4);
			float aQ = luaL_checknumber(L, 5);
			float cutoff = luaL_checknumber(L, 6);

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);
			vec3 *e = reinterpret_cast<vec3 *>(address_emission);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initSpotLight(*t, *e, aC, aL, aQ, cutoff));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewMesh_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_material = static_cast<std::size_t>(luaL_checkinteger(L, 2));
			std::string filename(luaL_checkstring(L, 3));

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);
			Material *m = reinterpret_cast<Material *>(address_material);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initMesh(new Mesh(*t, m, g_scene->getObjFactory(), filename)));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewSphere_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			std::size_t address_material = static_cast<std::size_t>(luaL_checkinteger(L, 2));
			std::size_t address_center_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 3));
			float r = luaL_checknumber(L, 4);

			// Get the objects from address 
			Transform *t = reinterpret_cast<Transform *>(address_transform);
			Material *m = reinterpret_cast<Material *>(address_material);
			vec3 *c = reinterpret_cast<vec3 *>(address_center_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initShape(new Sphere(*t, m, *c, r)));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewDiffMaterial_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_diff_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the vector from address 
			vec3 *diff_vec3 = reinterpret_cast<vec3 *>(address_diff_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDiffuseMaterial("", *diff_vec3, vec3(0, 0, 0)));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewEmisMaterial_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_emis_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the vector from address 
			vec3 *emis_vec3 = reinterpret_cast<vec3 *>(address_emis_vec3);

			// Create the desired object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDiffuseMaterial("", vec3(0, 0, 0), *emis_vec3));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewDielectricMaterial_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_diff_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			float ior = luaL_checknumber(L, 2);

			// Get the vector from address 
			vec3 *diff_vec3 = reinterpret_cast<vec3 *>(address_diff_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDielectricMaterial(*diff_vec3, vec3(), vec3(), ior));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewSpecMaterial_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_diff_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the vector from address 
			vec3 *diff_vec3 = reinterpret_cast<vec3 *>(address_diff_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initSpecularMaterial(*diff_vec3, vec3(), vec3()));
			lua_pushinteger(L, address);

			return 1;
		}

		int lua_NewGlossyMaterial_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_diff_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));
			float r = luaL_checknumber(L, 2);
			float k = luaL_checknumber(L, 3);
			float d = luaL_checknumber(L, 4);

			// Get the vector from address 
			vec3 *diff_vec3 = reinterpret_cast<vec3 *>(address_diff_vec3);

			// Create the object and return the address 
			std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initGlossyMaterial(*diff_vec3, vec3(), vec3(), r, k, d));
			lua_pushinteger(L, address);

			return 1;
		}

		// C++ Scene object adders
		int lua_AddCamera_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_camera = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the object from address 
			Camera *camera = reinterpret_cast<Camera *>(address_camera);

			// Add the object to scene 
			g_scene->addCamera(camera);

			MLOG_INFO("Lua: a New Camera was added to the current scene.");

			return 0;
		}

		int lua_AddLight_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_light = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the object from address 
			Light *light = reinterpret_cast<Light *>(address_light);

			// Add the object to scene 
			g_scene->addLight(light);

			MLOG_INFO("Lua: a New Light was added to the current scene.");

			return 0;
		}

		int lua_AddMesh_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_mesh = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the object from address 
			Mesh *mesh = reinterpret_cast<Mesh *>(address_mesh);

			// Add the object to scene 
			g_scene->addMesh(mesh);

			MLOG_INFO("Lua: a New Mesh was added to the current scene.");

			return 0;
		}

		int lua_AddShape_func(lua_State * L)
		{
			// Get the function arguments 
			std::size_t address_shape = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the object from address 
			Shape *shape = reinterpret_cast<Shape *>(address_shape);

			// Add the object to scene 
			g_scene->addShape(shape);

			MLOG_INFO("Lua: a New Shape was added to the current scene.");

			return 0;
		}

		int lua_AddRayAccelerator_func(lua_State * L)
		{
			// Get the function arguments 
			std::string type(luaL_checkstring(L, 1));
			int param1 = luaL_checknumber(L, 2);

			// Check if an accelerator exists already 
			if (g_scene->getAccelerator())
			{
				MLOG_ERROR("Lua: a Ray acceleration structure already exists for the current scene!");
				return 0;
			}

			// Create the object and add it to the scene 
			if (type == "bvh")
			{
				Accelerator *accel = new BVHAccel(g_scene->getShapes(), param1);
				accel->init();
				g_scene->setAccelerator(accel);

				MLOG_INFO("Lua: a BVHAccel was added to the current scene. Leaf threshold: %d.", param1);
			}
			else
			{
				MLOG_ERROR("Lua: Invalid ray acceleration structure type. Accelerator was not created.");
			}

			return 0;
		}

		extern int lua_SetDisplayInitInfo_func(lua_State * L)
		{
			// Get the function arguments 
			g_dispInitInfo.width = luaL_checknumber(L, 1);
			g_dispInitInfo.height = luaL_checknumber(L, 2);
			g_dispInitInfo.scale = luaL_checknumber(L, 3);

			MLOG_INFO("Lua: Set display init info to: [%d, %d, %d].",
				g_dispInitInfo.width,
				g_dispInitInfo.height,
				g_dispInitInfo.scale
			);

			return 0;
		}

		extern int lua_SetMThreadInitInfo_func(lua_State * L)
		{
			// Get the function argument(s) 
			g_mThreadInitInfo.rThreadCount = luaL_checknumber(L, 1);

			MLOG_INFO("Lua: Set multithreading init info to %d.", g_mThreadInitInfo.rThreadCount);

			return 0;
		}

		extern int lua_SetRadianceClamping_func(lua_State * L)
		{
			// Get the function argument 
			int n = luaL_checknumber(L, 1);

			// Set the variable 
			g_scene->setRadianceClamping(n);

			MLOG_INFO("Lua: Set scene radiance clamping to %d.", n);

			return 0;
		}

		extern int lua_SetMaxRecursion_func(lua_State * L)
		{
			// Get the function argument 
			int n = luaL_checkinteger(L, 1);

			// Set the variable 
			g_scene->setMaxRecursion(n);

			MLOG_INFO("Lua: Set scene maximum recursion for rays to %d.", n);

			return 0;
		}

		extern int lua_SetSceneSkyColor_func(lua_State * L)
		{
			// Get the function argument
			std::size_t address_col_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));

			// Get the objects from address
			vec3 * col_vec3 = reinterpret_cast<vec3 *>(address_col_vec3);

			// Set the variable
			g_scene->setSkyColor(*col_vec3);

			MLOG_INFO("Lua: Set scene sky color to: %s", col_vec3->toString().c_str());

			return 0;
		}

	}

}
