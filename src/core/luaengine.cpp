
// std includes
#include <iostream>

// mirage includes
#include "luaengine.h"
#include "../macros.h"

namespace mirage
{

namespace lua
{

lua_State *g_state;

void init()
{
    if (!g_state)
    {
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

}

}
