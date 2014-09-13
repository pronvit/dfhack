#pragma once
#include "Error.h"
#include "Export.h"
#include <string>
#include <unordered_map>

namespace DFHack {
    namespace Colors {
        struct ColorScheme {
            float ccolor[16][3];
        };
        DFHACK_EXPORT bool schemeExists (std::string name);
        DFHACK_EXPORT bool loadScheme (std::string name, bool refresh);
        DFHACK_EXPORT bool saveScheme (std::string name, bool overwrite);
    }
}
