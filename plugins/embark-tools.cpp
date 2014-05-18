#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"

using namespace DFHack;

std::map<std::string, bool> enabled_tools;
std::map<std::string, std::string> tool_names;

command_result embark_tools (color_ostream &out, std::vector <std::string> & parameters);

DFHACK_PLUGIN("embark-tools");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);

DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
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

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
    return CR_OK;
}

DFhackCExport command_result plugin_enable ( color_ostream &out, bool enable)
{
    if (is_enabled != enable)
    {
        is_enabled = enable;
    }

    return CR_OK;
}

command_result embark_tools (color_ostream &out, std::vector <std::string> & parameters)
{
    CoreSuspender suspend;
    if (parameters.size())
    {
        bool enable_state = true;
        for (size_t i = 0; i < parameters.size(); i++)
        {
            if (parameters[i] == "enable")
            {
                enable_state = true;
                is_enabled = true;  // Enable plugin
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
