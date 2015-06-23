#include "ColorText.h"

struct lua_State;
using namespace DFHack;

namespace DFHack {
    namespace Lua {
        lua_State *Open(color_ostream &out, lua_State *state);
    }
}

extern "C" {
    lua_State *luai_open(color_ostream &out, lua_State *state) {
        return Lua::Open(out, state);
    }
}
