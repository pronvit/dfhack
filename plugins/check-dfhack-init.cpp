#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"

#include "modules/Filesystem.h"
#include "modules/Gui.h"
#include "modules/Screen.h"

#include "uicommon.h"

using namespace DFHack;

command_result check_dfhack_init (color_ostream &out, std::vector <std::string> & parameters);

DFHACK_PLUGIN("check-dfhack-init");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);

class dfhack_init_prompt : public dfhack_viewscreen
{
public:
    std::string message;
    UIColor message_color;
    dfhack_init_prompt():
        message(""),
        message_color(COLOR_LIGHTGREEN)
    { }
    void error (std::string err)
    {
        message = err;
        message_color = COLOR_LIGHTRED;
    }
    void feed (std::set<df::interface_key> *input) {
        if (message.size())
        {
            Screen::dismiss(this);
            return;
        }
        else if (DFHack::Filesystem::exists("dfhack.init"))
        {
            error("dfhack.init already exists");
            return;
        }
        else if (!DFHack::Filesystem::isfile("dfhack.init-example"))
        {
            error("dfhack.init-example not found");
            return;
        }
        else if (input->count(df::interface_key::CUSTOM_Y))
        {
            if (rename("dfhack.init-example", "dfhack.init"))
            {
                error("Failed to rename dfhack.init-example to dfhack.init");
            }
            else
            {
                message = "File renamed. You will need to restart DF for your changes to take effect.";
            };
        }
        else if (input->count(df::interface_key::LEAVESCREEN) ||
                 input->count(df::interface_key::CUSTOM_N))
        {
            is_enabled = false;
            Screen::dismiss(this);
        }
    }
    void render () {
        Screen::clear();
        int x = 2, y = 2;
        if (message.size())
        {
            OutputString(message_color, x, y, message, true, 2);
            y++;
            OutputString(COLOR_GREY, x, y, "Press any key", true, 2);
            return;
        }
        Screen::drawBorder(" dfhack.init not found ");
        OutputString(COLOR_WHITE, x, y, "You do not have \"dfhack.init\" in your DF folder, but you do have", true, 2);
        OutputString(COLOR_WHITE, x, y, "\"dfhack.init-example\". Would you like to rename it to \"dfhack.init\"?", true, 2);
        y++;
        OutputString(COLOR_WHITE, x, y, "(The contents of dfhack.init are run automatically when DFHack starts)", true, 2);
        y++;
        OutputHotkeyString(x, y, "Rename to dfhack.init", "y", true, 2);
        OutputHotkeyString(x, y, "Do not rename", "n", true, 2);
    }
    std::string getFocusString () { return "dfhack_init_prompt"; }
};

DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
    is_enabled = true;  // Disabled when cancelled
    commands.push_back(PluginCommand(
        "check-dfhack-init",
        "Checks for the existence of dfhack.init on startup",
        check_dfhack_init,
        false,
        "Checks for the existence of dfhack.init on startup"
    ));
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream &out)
{
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    is_enabled = enable;
    return CR_OK;
}

DFhackCExport command_result plugin_onstatechange(color_ostream &out, state_change_event e)
{
    
    if (is_enabled && e == DFHack::SC_VIEWSCREEN_CHANGED && Gui::getCurFocus() == "title")
    {
        if (!DFHack::Filesystem::exists("dfhack.init") &&
            DFHack::Filesystem::isfile("dfhack.init-example"))
        {
            Screen::show(new dfhack_init_prompt);
        }
    }
    return CR_OK;
}

command_result check_dfhack_init (color_ostream &out, std::vector <std::string> & parameters)
{
    if (!parameters.empty())
        return CR_WRONG_USAGE;
    CoreSuspender suspend;
    if (!DFHack::Filesystem::isfile("dfhack.init"))
    {
        out << "dfhack.init does not exist" << std::endl;
    }
    return CR_OK;
}

