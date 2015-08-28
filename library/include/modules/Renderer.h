#include "Export.h"
#include "df/enabler.h"
#include "df/renderer.h"
#pragma once

namespace DFHack { namespace Renderer {
    struct DFHACK_EXPORT renderer_wrap : public df::renderer {
        void set_to_null();
        void copy_from_parent();
        void copy_to_parent();
        renderer_wrap *parent;
        renderer_wrap *child;
    };
    DFHACK_EXPORT renderer_wrap *AddRenderer(renderer_wrap*);
    DFHACK_EXPORT void RemoveRenderer(renderer_wrap*);
    DFHACK_EXPORT bool RendererExists(renderer_wrap*);
    inline renderer_wrap *GetRenderer()
    {
        return (renderer_wrap*)(df::global::enabler ? df::global::enabler->renderer : NULL);
    }
}}
