#include "modules/Colors.h"
#include <VTableInterpose.h>
#include "df/enabler.h"
#include "df/graphic.h"

using namespace DFHack::Colors;
using df::global::enabler;
using df::global::gps;

static std::unordered_map<std::string, ColorScheme> color_schemes;

void refresh_display() { gps->force_full_display_count++; }

bool DFHack::Colors::schemeExists (std::string name)
{
    return (color_schemes.find(name) != color_schemes.end());
}

bool DFHack::Colors::loadScheme (std::string name, bool refresh)
{
    auto item = color_schemes.find(name);
    if (item != color_schemes.end())
    {
        ColorScheme scheme = item->second; 
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                enabler->ccolor[i][j] = scheme.ccolor[i][j];
            }
        }
        if (refresh)
            refresh_display();
        return true;
    }
    else return false;
}

bool DFHack::Colors::saveScheme (std::string name, bool overwrite)
{
    if (overwrite || (color_schemes.find(name) == color_schemes.end()))
    {
        ColorScheme scheme;
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                scheme.ccolor[i][j] = enabler->ccolor[i][j];
            }
        }
        color_schemes[name] = scheme;
        return true;
    }
    else return false;
}
