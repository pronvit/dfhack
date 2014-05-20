#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"

#include <set>

#include <modules/Screen.h>
#include <VTableInterpose.h>

#include "ColorText.h"
#include "df/graphic.h"
#include "df/viewscreen_titlest.h"
#include "df/viewscreen_loadgamest.h"

using namespace DFHack;

using df::global::gps;

command_result cmd_load_screen (color_ostream &out, std::vector <std::string> & parameters);

class viewscreen_load_screen : public dfhack_viewscreen
{
public:
    void feed (std::set<df::interface_key> *input);
    void render ();
    void help () { };
    std::string getFocusString() { return "loadscreen"; };
    viewscreen_load_screen () { };
    ~viewscreen_load_screen () { };
};

void viewscreen_load_screen::feed (std::set<df::interface_key> *input)
{
    for (auto iter = input->begin(); iter != input->end(); iter++)
    {
        df::interface_key key = *iter;
        if (key == df::interface_key::LEAVESCREEN)
        {
            Screen::dismiss(this);
            Screen::dismiss(this->parent);
            return;
        }
    }    
}

void viewscreen_load_screen::render ()
{
    Screen::clear();
    Screen::paintString(Screen::Pen(' ', COLOR_RED, COLOR_BLUE), 0, 0, "Load menu");
}

struct loadgame_hooks : df::viewscreen_loadgamest
{
    typedef df::viewscreen_loadgamest interpose_base;
    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        Screen::show(new viewscreen_load_screen);
    }
};

IMPLEMENT_VMETHOD_INTERPOSE(loadgame_hooks, render);

DFHACK_PLUGIN("load-screen");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);

DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    if (!gps)
        return CR_FAILURE;
    if (enable != is_enabled)
    {
        if (!INTERPOSE_HOOK(loadgame_hooks, render).apply(enable))
            return CR_FAILURE;
        is_enabled = enable;
    }
    return CR_OK;
}

