
// std includes
#include <iostream>
#include <cstring>

// mirage includes
#include "luaengine.h"
#include "../macros.h"

namespace mirage
{

namespace lua
{

lua_State *g_state;
Scene *g_scene;

void init(Scene *scene)
{
    if (!g_state)
    {
        g_scene = scene;
        g_state = luaL_newstate();

        luaopen_io(g_state);
        luaopen_base(g_state);
        luaopen_table(g_state);
        luaopen_string(g_state);
        luaopen_math(g_state);
        luaL_openlibs(g_state);

        LOG("Lua: Scripting engine initialized successfully. (" << LUA_VERSION << ")");
    }
    else
    {
        ERR("Lua: There was a problem initializing " << LUA_VERSION << "...");
        std::exit(1);
    }
}

void kill()
{
    if (g_state)
    {
        lua_close(g_state);

        LOG("Lua: Sripting engine unloaded successfully.");
    }
    else
    {
        ERR("Lua: There was a problem unloading " << LUA_VERSION << "...");
        std::exit(1);
    }
}

void rerr(int status)
{
    if (g_state)
    {
        if (status != 0)
        {
            ERR("Lua: " << lua_tostring(g_state, -1));
            lua_pop(g_state, 1);
        }
    }
}

void load(const std::string filename)
{
    if (g_state)
    {
        /* Load the lua script into memory */
        int status = luaL_loadfile(g_state, filename.c_str());

        /* Register available C/C++ functions & objects */
        lua_pushcfunction(g_state, lua_print_func);
        lua_setglobal(g_state, "print");
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
        lua_pushcfunction(g_state, lua_NewDiffMaterial_func);
        lua_setglobal(g_state, "NewDiffMaterial");
        lua_pushcfunction(g_state, lua_NewEmisMaterial_func);
        lua_setglobal(g_state, "NewEmisMaterial");

        /* Execute the program if no errors found */
        if (status == 0)
        {
            /* Initially run the script once */
            lua_call(g_state, 0, 0);

            /* Execute the init function */
            exec(filename, "init");
        }

        /* Report found errors */
        rerr(status);
    }
}

void exec(const std::string filename, const std::string funcname)
{
    /* Get the desired function from the environment */
    lua_getglobal(g_state, funcname.c_str());

    /* Call the desired function */
    lua_call(g_state, 0, 0);
}

int lua_print_func(lua_State *L)
{
    /* Get the function argument */
    const char *str = luaL_checkstring(L, 1);

    /* Call the underlying C++ function */
    LOG("LuaScript: " << str);

    return 0;
}

int lua_NewVector3_func(lua_State *L)
{
    /* Get the function arguments */
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float z = luaL_checknumber(L, 3);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initVector3(x, y, z));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewVector4_func(lua_State *L)
{
    /* Get the function arguments */
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float z = luaL_checknumber(L, 3);
    float w = luaL_checknumber(L, 4);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initVector4(x, y, z, w));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewQuaternion_func(lua_State *L)
{
    /* Get the function arguments */
    float w = luaL_checknumber(L, 1);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_checknumber(L, 4);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initQuaternion(w, x, y, z));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewQuaternionLookAt_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_origin_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));
    std::size_t address_target_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 2));

    /* Get the objects from address */
    vec3 *origin_vec3 = reinterpret_cast<vec3 *>(address_origin_vec3);
    vec3 *target_vec3 = reinterpret_cast<vec3 *>(address_target_vec3);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initQuaternion(*origin_vec3, *target_vec3));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewTransform_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_pos_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));
    std::size_t address_ori_quat = static_cast<std::size_t>(luaL_checkinteger(L, 2));
    std::size_t address_scl_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 3));

    /* Get the objects from address */
    vec3 *pos_vec3 = reinterpret_cast<vec3 *>(address_pos_vec3);
    quaternion *ori_quat = reinterpret_cast<quaternion *>(address_ori_quat);
    vec3 *scl_vec3 = reinterpret_cast<vec3 *>(address_scl_vec3);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initTransform(*pos_vec3, *ori_quat, *scl_vec3));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewCameraOrtho_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
    float speed = luaL_checknumber(L, 2);
    float sensitivity = luaL_checknumber(L, 3);
    float zoom = luaL_checknumber(L, 4);

    /* Get the objects from address */
    Transform *t = reinterpret_cast<Transform *>(address_transform);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initOrthoCamera(*t, speed, sensitivity, zoom));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewCameraPersp_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
    float speed = luaL_checknumber(L, 2);
    float sensitivity = luaL_checknumber(L, 3);
    float fov = luaL_checknumber(L, 4);

    /* Get the objects from address */
    Transform *t = reinterpret_cast<Transform *>(address_transform);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initPerspCamera(*t, speed, sensitivity, fov));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewLightDir_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_transform = static_cast<std::size_t>(luaL_checkinteger(L, 1));
    std::size_t address_emission = static_cast<std::size_t>(luaL_checkinteger(L, 2));

    /* Get the objects from address */
    Transform *t = reinterpret_cast<Transform *>(address_transform);
    vec3 *e = reinterpret_cast<vec3 *>(address_emission);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDirLight(*t, *e));
    lua_pushinteger(L, address);

    return 1;
}

//int lua_NewLightPoint_func(lua_State *L);
//int lua_NewLightSpot_func(lua_State *L);

int lua_NewDiffMaterial_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_diff_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));

    /* Get the vector from address */
    vec3 *diff_vec3 = reinterpret_cast<vec3 *>(address_diff_vec3);

    /* Create the object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDiffuseMaterial(*diff_vec3, vec3(0, 0, 0)));
    lua_pushinteger(L, address);

    return 1;
}

int lua_NewEmisMaterial_func(lua_State *L)
{
    /* Get the function arguments */
    std::size_t address_emis_vec3 = static_cast<std::size_t>(luaL_checkinteger(L, 1));

    /* Get the vector from address */
    vec3 *emis_vec3 = reinterpret_cast<vec3 *>(address_emis_vec3);

    /* Create the desired object and return the address */
    std::size_t address = reinterpret_cast<std::size_t>(g_scene->getObjFactory()->initDiffuseMaterial(vec3(0, 0, 0), *emis_vec3));
    lua_pushinteger(L, address);

    return 1;
}

}

}
