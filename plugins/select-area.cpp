#include <vector>
#include <set>

#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"
//#include "ColorText.h"
#include "uicommon.h"
#include "VTableInterpose.h"

#include "modules/Gui.h"
#include "modules/Maps.h"

#include "df/interface_key.h"
#include "df/ui.h"
#include "df/viewscreen_dwarfmodest.h"

using namespace DFHack;
using namespace df::enums;

using df::global::ui;

DFHACK_PLUGIN("select-area");
DFHACK_PLUGIN_IS_ENABLED(enabled);

class select_area_sidebar : public dfhack_viewscreen
{
public:
    std::string getFocusString() { return "SelectArea"; }
    void render()
    {
        Screen::clear();
    };
    void feed(std::set<df::interface_key> *input)
    {
        if (input->count(interface_key::LEAVESCREEN))
        {
            Screen::dismiss(this);
        }
    }
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
                //CoreSuspendClaimer suspend;
                //color_ostream out = Core::getInstance().getConsole();
                //Core::getInstance().runCommand(Core::getInstance().getConsole(), "gui/select-area");
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
            int x = dims.menu_x1 + 1,
                y = 21;
            OutputString(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(interface_key::CUSTOM_E));
            OutputString(COLOR_WHITE, x, y, ": Select area");
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
