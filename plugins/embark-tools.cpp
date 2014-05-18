#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"

#include <modules/Screen.h>
#include <set>

#include <VTableInterpose.h>
#include "ColorText.h"
#include "df/viewscreen_choose_start_sitest.h"
#include "df/interface_key.h"

using namespace DFHack;

std::map<std::string, bool> enabled_tools;
std::map<std::string, std::string> tool_names;

command_result embark_tools (color_ostream &out, std::vector <std::string> & parameters);

/*
 * Viewscreen hooks
 */

void OutputString(int8_t color, int &x, int y, const std::string &text)
{
    Screen::paintString(Screen::Pen(' ', color, 0), x, y, text);
    x += text.length();
}

struct choose_start_site_hook : df::viewscreen_choose_start_sitest
{
    typedef df::viewscreen_choose_start_sitest interpose_base;

    DEFINE_VMETHOD_INTERPOSE(void, feed, (std::set<df::interface_key> *input))
    {
        INTERPOSE_NEXT(feed)(input);
    }

    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        INTERPOSE_NEXT(render)();
        auto dim = Screen::getWindowSize();
        int x = 1,
            y = dim.y - 5;
        OutputString(COLOR_LIGHTMAGENTA, x, y, "Enabled:");
    }
};

IMPLEMENT_VMETHOD_INTERPOSE(choose_start_site_hook, feed);
IMPLEMENT_VMETHOD_INTERPOSE(choose_start_site_hook, render);

DFHACK_PLUGIN("embark-tools");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);

DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
    commands.push_back(PluginCommand(
        "embark-tools",
        "A collection of embark tools",
        embark_tools,
        false,
        "embark-tools (enable/disable) tool [tool...]\n"
        "Tools:\n"
        "  anywhere: Allows embarking anywhere\n"
        "  nano: Allows the embark size to be decreased below 2x2\n"
    ));
    enabled_tools["anywhere"] = false;
    enabled_tools["nano"] = false;
    tool_names["anywhere"] = "Embark anywhere";
    tool_names["nano"] = "Nano embark";
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream &out)
{
    INTERPOSE_HOOK(choose_start_site_hook, feed).remove();
    INTERPOSE_HOOK(choose_start_site_hook, render).remove();
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    if (is_enabled != enable)
    {
        if (!INTERPOSE_HOOK(choose_start_site_hook, feed).apply(enable) ||
            !INTERPOSE_HOOK(choose_start_site_hook, render).apply(enable))
             return CR_FAILURE;
        is_enabled = enable;
    }
    return CR_OK;
}

command_result embark_tools (color_ostream &out, std::vector <std::string> & parameters)
{
    CoreSuspender suspend;
    if (parameters.size())
    {
        // Set by "enable"/"disable" - allows for multiple commands, e.g. "enable nano disable anywhere"
        bool enable_state = true;
        for (size_t i = 0; i < parameters.size(); i++)
        {
            if (parameters[i] == "enable")
            {
                enable_state = true;
                plugin_enable(out, true);  // Enable plugin
            }
            else if (parameters[i] == "disable")
                enable_state = false;
            else if (enabled_tools.count(parameters[i]))
            {
                enabled_tools[parameters[i]] = enable_state;
                out << (enable_state ? "Enabled" : "Disabled") << " " <<
                    tool_names[parameters[i]] << std::endl;
            }
            else
                return CR_WRONG_USAGE;
        }
    }
    else
    {
        out << "Tool status:" << std::endl;
        for (auto iter = enabled_tools.begin(); iter != enabled_tools.end(); iter++)
        {
            out << tool_names[iter->first] << " (" << iter->first << ")";
            out << ": " << (iter->second ? "Enabled" : "Disabled") << std::endl;
        }
    }
    return CR_OK;
}
