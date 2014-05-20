#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"

#include <set>

#include <modules/Gui.h>
#include <modules/Screen.h>
#include <VTableInterpose.h>

#include "ColorText.h"
#include "df/graphic.h"
#include "df/viewscreen_titlest.h"
#include "df/viewscreen_loadgamest.h"

using namespace DFHack;

using df::global::gps;

std::string mode_string (df::game_type gametype);

command_result cmd_load_screen (color_ostream &out, std::vector <std::string> & parameters);

std::string mode_string (df::game_type gametype)
{
    switch (gametype)
    {
        case df::game_type::DWARF_MAIN:
            return "Fortress mode";
        case df::game_type::DWARF_RECLAIM:
            return "Reclaim fortress mode";
        case df::game_type::ADVENTURE_MAIN:
            return "Adventure mode";
        default:
            return "";
    }
}

class viewscreen_load_screen : public dfhack_viewscreen
{
public:
    void feed (std::set<df::interface_key> *input);
    void render ();
    void help () { };
    std::string getFocusString() { return "loadscreen"; };
    viewscreen_load_screen ();
    ~viewscreen_load_screen () { };
    df::viewscreen_loadgamest* parent_screen ();
protected:
    int sel_idx;
    std::vector<df::viewscreen_loadgamest::T_saves> saves;
};

viewscreen_load_screen::viewscreen_load_screen ()
{
    sel_idx = 0;
}

df::viewscreen_loadgamest* viewscreen_load_screen::parent_screen ()
{
    df::viewscreen * top = this->parent;
    VIRTUAL_CAST_VAR(parent_screen, df::viewscreen_loadgamest, top);
    return parent_screen;
}

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
    auto dim = Screen::getWindowSize();
    df::viewscreen_loadgamest* parent = parent_screen();
    if (!parent)
        return;
    Screen::clear();
    std::string title = "Load game (DFHack)";
    Screen::paintString(Screen::Pen(' ', COLOR_WHITE, COLOR_BLACK),
                        (dim.x / 2) - (title.length() / 2),
                        0,
                        title);
    auto games = parent->saves;
    int row = 2, i = 0;
    for (auto iter = games.begin(); iter != games.end() && row + 2 < dim.y; iter++, row += 2, i++)
    {
        df::viewscreen_loadgamest::T_saves * save = *iter;
        color_value fg = (i == sel_idx) ? COLOR_WHITE : COLOR_GREY;
        auto pen = Screen::Pen(' ', fg, COLOR_BLACK);
        Screen::paintString(pen, 2, row, save->fort_name + " - " + mode_string(save->game_type));
        Screen::paintString(pen, 3, row + 1, "Folder: " + save->folder_name);
    }
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

