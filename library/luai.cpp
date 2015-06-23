#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "DataFuncs.h"
#include "modules/Filesystem.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static const char *DFHACK_LIB_PATH =
    #ifdef WIN32
        "../SDL.dll";
    #elif defined(_DARWIN)
        "./libdfhack.dylib";
    #else
        "./libdfhack.so";
    #endif

namespace DFHack {
    struct DFLibrary;
    DFHACK_EXPORT DFLibrary * OpenPlugin (const char * filename);
    DFHACK_EXPORT void * LookupPlugin (DFLibrary * plugin ,const char * function);
    DFHACK_EXPORT void ClosePlugin (DFLibrary * plugin);
}

using namespace DFHack;

lua_State* (*luai_open)(color_ostream &out, lua_State *state) = NULL;

static char *progname;
static void fatal (const char *format, ...) {
    if (progname) fprintf(stderr, "%s: ", progname);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
    exit(EXIT_FAILURE);
}

static int pmain (lua_State *L) {
    int status;
    int argc = (int)lua_tointeger(L, 1);
    int narg = argc - 2;
    char **argv = (char **)lua_touserdata(L, 2);
    char *script = argv[1];
    /* open standard libraries */
    luaL_checkversion(L);
    lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
    luaL_openlibs(L);  /* open libraries */
    lua_gc(L, LUA_GCRESTART, 0);
    /* execute main script */
    status = luaL_loadfile(L, script);
    if (status != LUA_OK)
        fatal("could not load script: %s", script);
    if (!lua_checkstack(L, argc + 1))
        fatal("too many arguments to script: %i", narg);
    for (int i = 2; i < argc; i++)
        lua_pushstring(L, argv[i]);
    return lua_pcall(L, narg, LUA_MULTRET, lua_gettop(L) - narg);
}

int main (int argc, char **argv) {
    DFLibrary *lib = OpenPlugin(DFHACK_LIB_PATH);
    if (!lib)
        fatal("cannot load DFHack library");
    #define bind(sym) \
        if (!(*((void**)&sym) = LookupPlugin(lib, #sym))) \
            fatal("cannot find symbol: " # sym)
    bind(luai_open);
    #undef bind
    progname = argv[0];
    int status, result;
    lua_State *L = luaL_newstate();  /* create state */
    if (L == NULL)
        fatal("cannot create state");
    if (argc < 2)
        fatal("no script specified");
    /* call 'pmain' in protected mode */
    lua_pushcfunction(L, &pmain);
    lua_pushinteger(L, argc);
    lua_pushlightuserdata(L, argv);
    lua_pcall(L, 2, 1, 0);
    lua_close(L);
    return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}
