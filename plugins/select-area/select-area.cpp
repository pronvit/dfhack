#include <vector>
#include <set>
#include <string>

#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"
#include "../uicommon.h"
#include "VTableInterpose.h"

#include "modules/Gui.h"
#include "modules/Maps.h"

#include "df/interface_key.h"
#include "df/ui.h"
#include "df/viewscreen_dwarfmodest.h"

#include "select-area.h"

using namespace DFHack;
using namespace df::enums;

using df::global::ui;

DFHACK_PLUGIN("select-area");
DFHACK_PLUGIN_IS_ENABLED(enabled);

void select_area_sidebar::change_state (select_area_sidebar_state new_state)
{
    state = new_state;
}

void select_area_sidebar::display_menu (select_area_sidebar_state state)
{
    int x,
        y = 3;
    for (int i = 0; i < sizeof(menu_options) / sizeof(menu_option); i++)
    {
        menu_option opt = menu_options[i];
        if (opt.state == state)
        {
            x = menu_x_offset() + 1;
            OutputString(COLOR_LIGHTGREEN, x, y, Screen::getKeyDisplay(opt.key));
            OutputString(COLOR_WHITE, x, y, ": " + opt.title);
            y++;
        }
    }
}

void select_area_sidebar::feed (std::set<df::interface_key> *input)
{
    if (input->count(interface_key::LEAVESCREEN))
    {
        Screen::dismiss(this);
    }
}

void select_area_sidebar::render()
{
    parent->render();
    if (!clear_sidebar_menu())
        return;
    int x = menu_x_offset() + 1,
        y = 2;
    display_menu(state);
    OutputString(COLOR_WHITE, x, y, "Select area (DFHack)");
};


struct dwarfmode_hook : df::viewscreen_dwarfmodest
{
    typedef viewscreen_dwarfmodest interpose_base;
    DEFINE_VMETHOD_INTERPOSE(void, feed, (std::set<df::interface_key> *input))
    {
        if (ui->main.mode == ui_sidebar_mode::Default)
        {
            if (input->count(interface_key::CUSTOM_E))
            {
                Screen::show(new select_area_sidebar);
            }
        }
        INTERPOSE_NEXT(feed)(input);
    }
    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        INTERPOSE_NEXT(render)();
        if (ui->main.mode == ui_sidebar_mode::Default)
        {
            auto dims = Gui::getDwarfmodeViewDims();
            if (dims.menu_x1 != -1)
            {
                int x = dims.menu_x1 + 1,
                    y = 21;
                OutputString(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(interface_key::CUSTOM_E));
                OutputString(COLOR_GREY, x, y, ": Select area");
            }
        }
    }
};

IMPLEMENT_VMETHOD_INTERPOSE(dwarfmode_hook, feed);
IMPLEMENT_VMETHOD_INTERPOSE(dwarfmode_hook, render);

static bool enable_hooks (bool enable)
{
    return INTERPOSE_HOOK(dwarfmode_hook, feed).apply(enable) &&
           INTERPOSE_HOOK(dwarfmode_hook, render).apply(enable);
}

DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream &out)
{
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    if (enabled == enable)
        return CR_OK;
    if (enable_hooks(enable))
    {
        enabled = enable;
        return CR_OK;
    }
    else
        return CR_FAILURE;
}
