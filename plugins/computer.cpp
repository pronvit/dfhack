#include <map>
#include <set>
#include <queue>

#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Error.h"
#include "Export.h"
#include "LuaTools.h"
#include "LuaWrapper.h"
#include "PluginManager.h"
#include "VTableInterpose.h"
#include "modules/Gui.h"
#include "modules/Screen.h"
#include "uicommon.h"

#include "df/building_drawbuffer.h"
#include "df/building_stockpilest.h"

using namespace DFHack;
using namespace df::enums;
using std::string;
using std::vector;

DFHACK_PLUGIN("computer");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);
REQUIRE_GLOBAL(gps);
REQUIRE_GLOBAL(ui);

typedef std::set<df::interface_key> ikey_set;

template <typename CT, typename Fn>
void for_each(CT &c, Fn func)
{
    std::for_each(c.begin(), c.end(), func);
}

template <typename T>
string to_string(T &value)
{
    std::string str;
    std::stringstream ss;
    ss << value;
    ss >> str;
    return str;
}

#define dbg Core::getInstance().getConsole()
;
class Display {
public:
    Display(df::building_stockpilest *bld)
        :bld(bld)
    {
        for (size_t i = 0; i < bld->room.width * bld->room.height; ++i)
            buffer.push_back(Screen::Pen('a', COLOR_LIGHTRED, COLOR_BLACK));
    }
    Screen::Pen get(size_t x, size_t y) const
    {
        int off = buffer_offset(x, y);
        if (off < 0)
            return Screen::Pen();
        return buffer[off];
    }
    void set(size_t x, size_t y, Screen::Pen pen)
    {
        int off = buffer_offset(x, y);
        if (off >= 0)
            buffer[off] = pen;
    }
    inline df::building_stockpilest *getBuilding() const { return bld; }
    bool operator< (const Display &other) const
    {
        return bld < other.bld;
    }
private:
    std::vector<Screen::Pen> buffer;
    inline int buffer_offset (size_t x, size_t y) const
    {
        int off = x + (bld->room.width * y);
        if (off >= 0 && off < bld->room.width * bld->room.height)
            return off;
        return -1;
    }
    df::building_stockpilest *bld;
};

class Computer {
public:
    Computer(int id)
        :id(id)
    { }
    std::set<Display> displays;

    int getId() { return id; }

    bool operator< (const Computer &other) const
    {
        return id < other.id;
    }
private:
    int id;
};

struct ComputerException : Error::All
{
    const char *what() { return "ComputerException"; }
};
struct BadComputerID : ComputerException
{
    int id;
    string msg;
    BadComputerID(int id):id(id)
    {
        msg = string("invalid computer ID: ") + to_string(id);
    }
    ~BadComputerID() throw() { }
    const char *what() const throw()
    {
        return msg.c_str();
    }
};

static std::set<Computer*> computers;
static int computer_next_id = 0;

namespace plua {
    static color_ostream_proxy *out;
    static lua_State *state;
    bool init()
    {
        out = new color_ostream_proxy(Core::getInstance().getConsole());
        state = Lua::Open(*out);
        return state;
    }
    void cleanup()
    {
        if (out)
        {
            delete out;
            out = NULL;
        }
        lua_close(state);
    }
    bool call (const char *func, int nargs = 0, int nres = 0)
    {
        if (!Lua::PushModulePublic(*out, state, "plugins.computer", func))
            return false;
        if (nargs > 0)
            lua_insert(state, lua_gettop(state) - nargs);
        return Lua::SafeCall(*out, state, nargs, nres);
    }
    bool simple_call (const char *func)
    {
        Lua::StackUnwinder top(state);
        return call(func);
    }
    template <typename T>
    void push (T val)
    {
        Lua::Push(state, val);
    }
    template <typename KeyType, typename ValueType>
    void table_set (lua_State *L, KeyType k, ValueType v)
    {
        Lua::Push(L, k);
        Lua::Push(L, v);
        lua_settable(L, -3);
    }
    namespace api {
        Computer *get_computer (int id)
        {
            for (auto it = computers.begin(); it != computers.end(); ++it)
                if ((*it)->getId() == id)
                    return *it;
            throw BadComputerID(id);
        }
        int new_computer()
        {
            int id = computer_next_id++;
            bool inserted;
            Computer *c = new Computer(id);
            std::tie(std::ignore, inserted) = computers.insert(c);
            if (!inserted)
            {
                delete c;
                return -1;
            }
            return id;
        }
        bool register_display (int id, df::building_stockpilest *stockpile)
        {
            CHECK_NULL_POINTER(stockpile);
            auto c = get_computer(id);
            bool inserted;
            std::tie(std::ignore, inserted) = c->displays.insert(Display(stockpile));
            return inserted;
        }

        int list_displays (lua_State *L)
        {
            auto c = get_computer(luaL_checkinteger(L, 1));
            lua_newtable(L);
            int i = 1;
            for_each(c->displays, [&](const Display &d) {
                table_set(L, i++, d.getBuilding());
            });
            return 1;
        }
    }
}

#define PLUGIN_LUA_FUNC(name) {"__" #name, df::wrap_function(plua::api::name, true)}
DFHACK_PLUGIN_LUA_FUNCTIONS {
    PLUGIN_LUA_FUNC(new_computer),
    PLUGIN_LUA_FUNC(register_display),
    DFHACK_LUA_END
};

#define PLUGIN_LUA_CMD(name) {"__" #name, plua::api::name}
DFHACK_PLUGIN_LUA_COMMANDS {
    PLUGIN_LUA_CMD(list_displays),
    DFHACK_LUA_END
};

struct comp_display_hook : df::building_stockpilest {
    typedef df::building_stockpilest interpose_base;
    DEFINE_VMETHOD_INTERPOSE(void, drawBuilding, (df::building_drawbuffer *db, int16_t unk))
    {
        INTERPOSE_NEXT(drawBuilding)(db, unk);
        for_each(computers, [&](const Computer *c)
        {
            for_each(c->displays, [&](const Display &d)
            {
                if (d.getBuilding() == this)
                {
                    for (int x = 0; x < room.width; ++x)
                    {
                        for (int y = 0; y < room.height; ++y)
                        {
                            Screen::Pen pen = d.get(x, y);
                            db->tile[x][y] = pen.ch;
                            db->fore[x][y] = pen.fg;
                            db->back[x][y] = pen.bg;
                            db->bright[x][y] = pen.bold;
                        }
                    }
                }
            });
        });
    }
};
IMPLEMENT_VMETHOD_INTERPOSE(comp_display_hook, drawBuilding);

DFhackCExport command_result plugin_init (color_ostream &out, vector <PluginCommand> &commands)
{
    if (!plua::init())
        return CR_FAILURE;
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    if (is_enabled != enable)
    {
        if (!INTERPOSE_HOOK(comp_display_hook, drawBuilding).apply(enable))
            return CR_FAILURE;
        is_enabled = enable;
    }
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream &out)
{
    if (plugin_enable(out, false) != CR_OK)
        return CR_FAILURE;
    plua::cleanup();
    return CR_OK;
}
