#ifndef LUAENGINE_H
#define LUAENGINE_H

// std includes
#include <iostream>
#include <string>

// mirage includes
#include "../macros.h"

// lua includes
extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace mirage
{

namespace lua
{

extern lua_State *g_state;

extern void init();
extern void kill();
extern void rerr(int status);
extern void load(const std::string filename);
extern void exec(const std::string filename, const std::string funcname);

}

}

#endif // LUAENGINE_H
