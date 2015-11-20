#include <map>

#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "LuaTools.h"
#include "LuaWrapper.h"
#include "PluginManager.h"
#include "modules/Filesystem.h"
#include "modules/Gui.h"
#include "modules/Maps.h"

#include "uicommon.h"
#include "df/block_square_event_designation_priorityst.h"
#include "df/ui_sidebar_menus.h"
#include "df/viewscreen_dwarfmodest.h"

using namespace DFHack;
using namespace df::enums;

DFHACK_PLUGIN("designation-history");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);
REQUIRE_GLOBAL(cursor);
REQUIRE_GLOBAL(selection_rect);
REQUIRE_GLOBAL(ui);
REQUIRE_GLOBAL(ui_sidebar_menus);

DFhackCExport command_result plugin_enable (color_ostream &out, bool state);

const char *describe_sidebar_mode (df::ui_sidebar_mode mode, const char *def = "Unknown")
{
    using namespace df::enums::ui_sidebar_mode;
    static std::map<df::ui_sidebar_mode, const char*> descs;
    if (descs.empty())
    {
        descs[DesignateMine]               = "Mine";
        descs[DesignateRemoveRamps]        = "Remove Up Stairs/Ramps";
        descs[DesignateUpStair]            = "Up Stair";
        descs[DesignateDownStair]          = "Down Stair";
        descs[DesignateUpDownStair]        = "Up/Down Stair";
        descs[DesignateUpRamp]             = "Up Ramp";
        descs[DesignateChannel]            = "Channel";
        descs[DesignateRemoveDesignation]  = "Remove Designation";
        descs[DesignateSmooth]             = "Smooth Stone";
        descs[DesignateCarveTrack]         = "Carve Track";
        descs[DesignateEngrave]            = "Engrave Stone";
        descs[DesignateCarveFortification] = "Carve Fortifications";
        descs[DesignateToggleEngravings]   = "Toggle Engravings";
        descs[DesignateToggleMarker]       = "Toggle Standard/Marking";
        descs[DesignateTrafficHigh]        = "Traffic: High";
        descs[DesignateTrafficNormal]      = "Traffic: Normal";
        descs[DesignateTrafficLow]         = "Traffic: Low";
        descs[DesignateTrafficRestricted]  = "Traffic: Restricted";
        descs[DesignateRemoveConstruction] = "Remove Construction";
    }
    return descs[mode] ? descs[mode] : def;
}

class Tile {
private:
    int x, y, z;
    uint8_t bx : 4;
    uint8_t by : 4;
    df::map_block *map_block;
    df::tile_dig_designation dig : 3;
    uint32_t smooth : 1;
    df::tile_traffic traffic : 2;
    int32_t priority;
    bool carve_track_north : 1;
    bool carve_track_south : 1;
    bool carve_track_east : 1;
    bool carve_track_west : 1;
    bool dig_marked : 1;
    bool dig_auto : 1;
    df::block_square_event_designation_priorityst *get_priority_event()
    {
        if (valid())
        {
            for (size_t i = 0; i < map_block->block_events.size(); ++i)
            {
                df::block_square_event *evt = map_block->block_events[i];
                if (evt->getType() == block_square_event_type::designation_priority)
                {
                    return (df::block_square_event_designation_priorityst*)evt;
                }
            }
        }
        return NULL;
    }
public:
    Tile (int x, int y, int z)
    {
        this->x = x; this->y = y; this->z = z;
        bx = x % 16; by = y % 16;
        map_block = Maps::getTileBlock(x, y, z);
        if (!map_block)
            return;
        df::tile_designation &des = map_block->designation[bx][by];
        df::tile_occupancy &occ = map_block->occupancy[bx][by];
        dig = des.bits.dig;
        smooth = des.bits.smooth;
        traffic = des.bits.traffic;
        auto evt = get_priority_event();
        priority = (evt) ? evt->priority[bx][by] : 0;
        carve_track_north = occ.bits.carve_track_north;
        carve_track_south = occ.bits.carve_track_south;
        carve_track_east = occ.bits.carve_track_east;
        carve_track_west = occ.bits.carve_track_west;
        dig_marked = occ.bits.dig_marked;
        dig_auto = occ.bits.dig_auto;
    }
    inline bool valid() { return map_block; }
    void write()
    {
        df::tile_designation &des = map_block->designation[bx][by];
        df::tile_occupancy &occ = map_block->occupancy[bx][by];
        des.bits.dig = dig;
        des.bits.smooth = smooth;
        des.bits.traffic = traffic;
        auto evt = get_priority_event();
        if (evt)
            evt->priority[bx][by] = priority;
        occ.bits.carve_track_north = carve_track_north;
        occ.bits.carve_track_south = carve_track_south;
        occ.bits.carve_track_east = carve_track_east;
        occ.bits.carve_track_west = carve_track_west;
        occ.bits.dig_marked = dig_marked;
        occ.bits.dig_auto = dig_auto;
    }
};

class Designation {
public:
    int x1, y1, z1, x2, y2, z2, dimx, dimy, dimz;
    df::ui_sidebar_mode mode;
    Tile *tiles;
    Designation (int x1_, int y1_, int z1_, int x2_, int y2_, int z2_, df::ui_sidebar_mode mode)
        :x1(x1_), y1(y1_), z1(z1_), x2(x2_), y2(y2_), z2(z2_),
         mode(mode)
    {
        if (x1 > x2)
            std::swap(x1, x2);
        if (y1 > y2)
            std::swap(y1, y2);
        if (z1 > z2)
            std::swap(z1, z2);
        dimx = x2 - x1 + 1;
        dimy = y2 - y1 + 1;
        dimz = z2 - z1 + 1;
        tiles = (Tile*)calloc(2 * dimx * dimy * dimz, sizeof(Tile));
    }

    ~Designation()
    {
        free(tiles);
    }

    void save_stage (int stage)
    {
        for (int x = x1; x <= x2; x++)
        {
            for (int y = y1; y <= y2; y++)
            {
                for (int z = z1; z <= z2; z++)
                {
                    new (tiles + tile_index(stage, x - x1, y - y1, z - z1)) Tile(x, y, z);
                }
            }
        }
    }

    void reset_stage (int stage)
    {
        for (int x = x1; x <= x2; x++)
        {
            for (int y = y1; y <= y2; y++)
            {
                for (int z = z1; z <= z2; z++)
                {
                    tiles[tile_index(stage, x - x1, y - y1, z - z1)].write();
                }
            }
        }
    }

    inline int tile_index (int stage, int x, int y, int z)
    {
        if (x < 0 || x >= dimx || y < 0 || y >= dimy || z < 0 || z >= dimz || stage < 0 || stage > 1) {
            return -1;
        }
        return (stage * dimx * dimy * dimz) + (x * dimy * dimz) + (y * dimz) + z;
    }

    std::string describe()
    {
        return std::string(describe_sidebar_mode(mode));
    }
};

static std::vector<Designation*> d_history;
static int d_history_idx;

static int erase_history(int start_index, int end_index)
{
    int max_index = d_history.size() - 1;
    if (start_index < 0 || start_index > max_index)
        return 1;
    if (end_index < 0 || end_index > max_index || end_index < start_index)
        return 2;
    d_history.erase(d_history.begin() + start_index, d_history.begin() + end_index + 1); // delete inclusively from start_index to end_index.
    if (d_history_idx >= start_index || d_history_idx <= end_index ||
            d_history_idx > d_history.size() - 1)
        d_history_idx = d_history.size() - 1;
    return 0;
}

static int64_t lua_last_mtime = -1;
const char *lua_filename = "hack/lua/plugins/designation-history.lua";

bool reload_lua()
{
    color_ostream &out = Core::getInstance().getConsole();
    auto L = Lua::Core::State;
    Lua::StackUnwinder top(L);
    if (!lua_checkstack(L, 4))
        return false;
    Lua::PushDFHack(L);
    lua_getfield(L, -1, "reload");
    lua_remove(L, -2);
    lua_pushstring(L, "plugins.designation-history");
    return Lua::SafeCall(out, L, 1, 0);
}

inline bool lua_safe_call (int nargs)
{
    auto L = Lua::Core::State;
    color_ostream &out = Core::getInstance().getConsole();
    return Lua::SafeCall(out, L, nargs, 0);
}

bool lua_init_call (const char *func, int stack_size = 0) {
    color_ostream &out = Core::getInstance().getConsole();
    int64_t cur_mtime = Filesystem::mtime(lua_filename);
    if (cur_mtime == -1)
    {
        out.printerr("Could not find %s\n", lua_filename);
        return false;
    }
    else if (cur_mtime != lua_last_mtime)
    {
        if (lua_last_mtime != -1 && !reload_lua())
        {
            out.printerr("Could not reload %s\n", lua_filename);
            return false;
        }
        lua_last_mtime = cur_mtime;
    }

    auto L = Lua::Core::State;

    if (!lua_checkstack(L, 1 + stack_size))
    {
        out.printerr("designation-history: No Lua stack space\n");
        return false;
    }

    if (!Lua::PushModulePublic(out, L, "plugins.designation-history", func))
        return false;

    return true;
}

bool lua_call_basic (const char *func)
{
    auto L = Lua::Core::State;
    Lua::StackUnwinder top(L);
    if (!lua_init_call(func))
        return false;
    return lua_safe_call(0);
}

bool lua_call_int_params (const char *func, std::vector<int> &params)
{
    auto L = Lua::Core::State;
    Lua::StackUnwinder top(L);
    if (!lua_init_call(func, params.size()))
        return false;
    for (size_t i = 0; i < params.size(); ++i)
        lua_pushinteger(L, params[i]);
    return lua_safe_call(params.size());
}

namespace DHLuaApi {
    template <typename T_key, typename T_value>
    void table_set (lua_State *L, T_key key, T_value value)
    {
        Lua::Push(L, key);
        Lua::Push(L, value);
        lua_settable(L, -3);
    }
    int get_history (lua_State *L)
    {
        lua_newtable(L);
        for (size_t i = 0; i < d_history.size(); i++)
        {
            Designation *d = d_history[i];
            lua_pushinteger(L, i + 1);
            lua_newtable(L);
            #define set_field(name) table_set(L, #name, d->name)
            set_field(x1);
            set_field(x2);
            set_field(dimx);
            set_field(y1);
            set_field(y2);
            set_field(dimy);
            set_field(z1);
            set_field(z2);
            set_field(dimz);
            set_field(mode);
            #undef set_field
            table_set(L, "centerx", (d->x1 + d->x2) / 2);
            table_set(L, "centery", (d->y1 + d->y2) / 2);
            table_set(L, "centerz", (d->z1 + d->z2) / 2);
            table_set(L, "id", i);
            table_set(L, "desc", d->describe().c_str());
            lua_settable(L, -3);
        }
        return 1;
    }
    int reset_stage (lua_State *L)
    {
        int idx = luaL_checkint(L, 1),
            stage = luaL_checkint(L, 2);
        if (stage != 0 && stage != 1)
            luaL_error(L, "invalid designation stage: %d", stage);
        if (idx < 0 || idx >= d_history.size())
            luaL_error(L, "invalid history index: %d", idx);
        Designation *d = d_history[idx];
        d->reset_stage(stage);
        return 0;
    }

    int remove_history (lua_State *L)
    {
        int start_index = luaL_checkint(L, 1),
            end_index = luaL_checkint(L, 2);
        int ret = erase_history(start_index - 1, end_index - 1);
        if (ret == 1)
            luaL_error(L, "remove_history: invalid start_index: %d (end_index: %d)", start_index, end_index);
        if (ret == 2)
            luaL_error(L, "remove_history: invalid end_index: %d (start_index: %d)", end_index, start_index);
        return 0;
    }
}

#define DH_LUA_FUNC(name) { #name, df::wrap_function(DHLuaApi::name,true) }
#define DH_LUA_CMD(name) { #name, DHLuaApi::name }
DFHACK_PLUGIN_LUA_COMMANDS {
    DH_LUA_CMD(get_history),
    DH_LUA_CMD(reset_stage),
    DH_LUA_CMD(remove_history),
    DFHACK_LUA_END
};

struct designation_menu_hook : df::viewscreen_dwarfmodest {
    typedef df::viewscreen_dwarfmodest interpose_base;
    bool valid_mode()
    {
        using namespace df::enums::ui_sidebar_mode;
        switch (ui->main.mode)
        {
        case DesignateMine:
        case DesignateRemoveRamps:
        case DesignateUpStair:
        case DesignateDownStair:
        case DesignateUpDownStair:
        case DesignateUpRamp:
        case DesignateChannel:
        case DesignateRemoveDesignation:
        case DesignateSmooth:
        case DesignateCarveTrack:
        case DesignateEngrave:
        case DesignateCarveFortification:
        case DesignateToggleEngravings:
        case DesignateToggleMarker:
        case DesignateTrafficHigh:
        case DesignateTrafficNormal:
        case DesignateTrafficLow:
        case DesignateTrafficRestricted:
        case DesignateRemoveConstruction:
            return true;
        }
        return false;
    }
    bool area_selection_mode()
    {
        if (valid_mode())
        {
            int32_t x, y, z;
            return Gui::getDesignationCoords(x, y, z);
        }
        return false;
    }
    inline bool can_undo()
    {
        return !d_history.empty() && d_history_idx >= 0 && d_history_idx < d_history.size();
    }
    inline bool can_redo()
    {
        return !d_history.empty() && d_history_idx + 1 < d_history.size();
    }
    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        INTERPOSE_NEXT(render)();
        if (valid_mode())
        {
            auto dims = Gui::getDwarfmodeViewDims();
            int x = dims.menu_x1 + 1;
            int y = 1;
            OutputString(COLOR_LIGHTRED, x, y, "Alt+U");
            OutputString(can_undo() ? COLOR_WHITE : COLOR_GREY, x, y, "ndo, ");
            OutputString(COLOR_LIGHTRED, x, y, "R");
            OutputString(can_redo() ? COLOR_WHITE : COLOR_GREY, x, y, "edo, ");
            OutputString(COLOR_LIGHTRED, x, y, "H");
            OutputString(COLOR_WHITE, x, y, "istory");
        }
    }
    DEFINE_VMETHOD_INTERPOSE(void, feed, (std::set<df::interface_key> *input))
    {
        if (valid_mode())
        {
            if (input->count(interface_key::CUSTOM_ALT_U))
            {
                if (can_undo())
                {
                    d_history[d_history_idx]->reset_stage(0);
                    --d_history_idx;
                }
            }
            else if (input->count(interface_key::CUSTOM_ALT_R))
            {
                if (can_redo())
                {
                    ++d_history_idx;
                    if (d_history_idx < 0)
                        d_history_idx = 0;
                    d_history[d_history_idx]->reset_stage(1);
                }
            }
            else if (input->count(interface_key::CUSTOM_ALT_H))
                lua_call_basic("history");
            else if (area_selection_mode() && input->count(interface_key::SELECT))
            {
                Designation* cur = new Designation(
                    selection_rect->start_x,
                    selection_rect->start_y,
                    selection_rect->start_z,
                    cursor->x,
                    cursor->y,
                    cursor->z,
                    ui->main.mode
                );
                cur->save_stage(0);
                INTERPOSE_NEXT(feed)(input);
                cur->save_stage(1);
                d_history.push_back(cur);
                d_history_idx = d_history.size() - 1;
            }
            else
            {
                INTERPOSE_NEXT(feed)(input);
            }
            ui_sidebar_menus->minimap.need_render = true;
        }
        else
        {
            INTERPOSE_NEXT(feed)(input);
        }
    }
};

IMPLEMENT_VMETHOD_INTERPOSE(designation_menu_hook, render);
IMPLEMENT_VMETHOD_INTERPOSE(designation_menu_hook, feed);

DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream &out)
{
    for (size_t i = 0; i < d_history.size(); i++)
        delete d_history[i];
    if (is_enabled)
        return plugin_enable(out, false);
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool state)
{
    if (state != is_enabled)
    {
        if (!INTERPOSE_HOOK(designation_menu_hook, render).apply(state) ||
            !INTERPOSE_HOOK(designation_menu_hook, feed).apply(state))
        {
            out.printerr("designation-history: Could not enable hooks\n");
            return CR_FAILURE;
        }
        is_enabled = state;
    }
    return CR_OK;
}

