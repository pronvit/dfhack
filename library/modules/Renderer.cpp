#include "DataDefs.h"
#include "MiscUtils.h"
#include "modules/Renderer.h"

using namespace DFHack;
using df::global::enabler;
using DFHack::Renderer::renderer_wrap;

static renderer_wrap *original_renderer = NULL;

bool init()
{
    if (!original_renderer)
        original_renderer = Renderer::GetRenderer();
    return original_renderer;
}

#define WHEREAMI Core::getInstance().getConsole().print("%s:%i\n", __PRETTY_FUNCTION__, __LINE__)
renderer_wrap *Renderer::AddRenderer (renderer_wrap *r)
{
    if (!init())
    {
        delete r;
        return NULL;
    }
    WHEREAMI;
    renderer_wrap *cur = GetRenderer();
    WHEREAMI;
    r->parent = cur;
    WHEREAMI;
    r->child = NULL;
    WHEREAMI;
    if (cur != original_renderer)
    {
        WHEREAMI;
        cur->child = original_renderer;
        WHEREAMI;
    }
    r->copy_from_parent();
    WHEREAMI;
    enabler->renderer = r;
    WHEREAMI;
    return r;
}

void Renderer::RemoveRenderer (renderer_wrap *r)
{
    if (r && original_renderer)
    {
        WHEREAMI;
        r->parent->child = r->child;
        WHEREAMI;
        if (r->child)
        {
            WHEREAMI;
            r->child->parent = r->parent;
            WHEREAMI;
        }
        enabler->renderer = r->parent;
        WHEREAMI;
    }
    WHEREAMI;
    delete r;
}

bool Renderer::RendererExists (renderer_wrap *r)
{
    renderer_wrap *cur = GetRenderer();
    while (cur && cur != r && cur != original_renderer)
        cur = cur->parent;
    return cur == r;
}

void Renderer::renderer_wrap::set_to_null() {
    WHEREAMI;
    screen = NULL;
    screentexpos = NULL;
    screentexpos_addcolor = NULL;
    screentexpos_grayscale = NULL;
    screentexpos_cf = NULL;
    screentexpos_cbr = NULL;
    screen_old = NULL;
    screentexpos_old = NULL;
    screentexpos_addcolor_old = NULL;
    screentexpos_grayscale_old = NULL;
    screentexpos_cf_old = NULL;
    screentexpos_cbr_old = NULL;
    WHEREAMI;
}

void Renderer::renderer_wrap::copy_from_parent() {
    WHEREAMI;
    screen = parent->screen;
    screentexpos = parent->screentexpos;
    screentexpos_addcolor = parent->screentexpos_addcolor;
    screentexpos_grayscale = parent->screentexpos_grayscale;
    screentexpos_cf = parent->screentexpos_cf;
    screentexpos_cbr = parent->screentexpos_cbr;
    screen_old = parent->screen_old;
    screentexpos_old = parent->screentexpos_old;
    screentexpos_addcolor_old = parent->screentexpos_addcolor_old;
    screentexpos_grayscale_old = parent->screentexpos_grayscale_old;
    screentexpos_cf_old = parent->screentexpos_cf_old;
    screentexpos_cbr_old = parent->screentexpos_cbr_old;
    WHEREAMI;
}

void Renderer::renderer_wrap::copy_to_parent() {
    WHEREAMI;
    parent->screen = screen;
    parent->screentexpos = screentexpos;
    parent->screentexpos_addcolor = screentexpos_addcolor;
    parent->screentexpos_grayscale = screentexpos_grayscale;
    parent->screentexpos_cf = screentexpos_cf;
    parent->screentexpos_cbr = screentexpos_cbr;
    parent->screen_old = screen_old;
    parent->screentexpos_old = screentexpos_old;
    parent->screentexpos_addcolor_old = screentexpos_addcolor_old;
    parent->screentexpos_grayscale_old = screentexpos_grayscale_old;
    parent->screentexpos_cf_old = screentexpos_cf_old;
    parent->screentexpos_cbr_old = screentexpos_cbr_old;
    WHEREAMI;
}
