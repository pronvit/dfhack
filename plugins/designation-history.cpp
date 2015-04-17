#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "LuaTools.h"
#include "PluginManager.h"
#include "modules/Filesystem.h"
#include "modules/Gui.h"

#include "uicommon.h"
#include "df/viewscreen_dwarfmodest.h"

using namespace DFHack;
using namespace df::enums;

DFHACK_PLUGIN("designation-history");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);
REQUIRE_GLOBAL(cursor);
REQUIRE_GLOBAL(selection_rect);
REQUIRE_GLOBAL(ui);

DFhackCExport command_result plugin_enable (color_ostream &out, bool state);

static int64_t lua_last_mtime = -1;
const char *lua_filename = "hack/lua/plugins/designation-history.lua";

bool reload_lua()
{
    color_ostream &out = Core::getInstance().getConsole();
    out.print("reload_lua");
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
    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        INTERPOSE_NEXT(render)();
        if (valid_mode())
        {
            auto dims = Gui::getDwarfmodeViewDims();
            int x = dims.menu_x1 + 1;
            int y = 1;
            OutputString(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(interface_key::CUSTOM_ALT_U));
            OutputString(COLOR_GREY, x, y, ": Undo, ");
            OutputString(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(interface_key::CUSTOM_ALT_H));
            OutputString(COLOR_GREY, x, y, ": History");
        }
    }
    DEFINE_VMETHOD_INTERPOSE(void, feed, (std::set<df::interface_key> *input))
    {
        if (valid_mode())
        {
            if (input->count(interface_key::CUSTOM_ALT_U))
                lua_call_basic("undo");
            else if (input->count(interface_key::CUSTOM_ALT_H))
                lua_call_basic("history");
            else
            {
                bool in_select = area_selection_mode() && input->count(interface_key::SELECT);
                std::vector<int> params;
                if (in_select)
                {
                    params.push_back(selection_rect->start_x);
                    params.push_back(selection_rect->start_y);
                    params.push_back(selection_rect->start_z);
                    params.push_back(cursor->x);
                    params.push_back(cursor->y);
                    params.push_back(cursor->z);
                    lua_call_int_params("before_select", params);
                }
                INTERPOSE_NEXT(feed)(input);
                if (in_select)
                    lua_call_int_params("after_select", params);
            }
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
    out.print("plugin_shutdown\n");
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

