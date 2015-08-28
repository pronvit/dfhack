#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"
#include "modules/Screen.h"
#include "modules/Renderer.h"

#include "df/enabler.h"
#include "df/renderer.h"

//#include "df/world.h"

using namespace DFHack;

DFHACK_PLUGIN("renderer-msg");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);
REQUIRE_GLOBAL(enabler);
REQUIRE_GLOBAL(gps);

#define WHEREAMI Core::getInstance().getConsole().print("%s:%i\n", __PRETTY_FUNCTION__, __LINE__)
struct renderer_msg : public Renderer::renderer_wrap {
    renderer_msg() {
        WHEREAMI;
    }
    virtual void update_tile(int32_t x, int32_t y) {
        static std::string str = std::string("DFHack: ") + plugin_name + " active";
        WHEREAMI;
        Screen::paintString(Screen::Pen(' ', 7, 1), 0, gps->dimy - 1, str);
        WHEREAMI;
        copy_to_parent();
        WHEREAMI;
        parent->update_tile(x,y);
        WHEREAMI;
    };
    virtual void update_all() {
        copy_to_parent();
        parent->update_all();
    };
    virtual void render() {
        copy_to_parent();
        parent->render();
    };
    virtual void set_fullscreen() {
        copy_to_parent();
        parent->set_fullscreen();
        copy_from_parent();
    };
    virtual void zoom(df::zoom_commands z) {
        copy_to_parent();
        parent->zoom(z);
        copy_from_parent();
    };
    virtual void resize(int32_t w, int32_t h) {
        copy_to_parent();
        parent->resize(w,h);
        copy_from_parent();
    };
    virtual void grid_resize(int32_t w, int32_t h) {
        copy_to_parent();
        parent->grid_resize(w,h);
        copy_from_parent();
    };
    virtual ~renderer_msg() {
        enabler->renderer=parent;
    };
    virtual bool get_mouse_coords(int32_t* x, int32_t* y) {
        return parent->get_mouse_coords(x,y);
    };
    virtual bool uses_opengl() {
        return parent->uses_opengl();
    };
protected:
    renderer* parent;
};

DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
    return CR_OK;
}

static Renderer::renderer_wrap *w = NULL;
DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    CoreSuspender s;
    is_enabled = enable;
    if (enable)
        w = Renderer::AddRenderer(new renderer_msg);
        //enabler->renderer = new renderer_msg(enabler->renderer);
    else
        Renderer::RemoveRenderer(w);
        //delete enabler->renderer;
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream &out)
{
    return plugin_enable(out, false);
}
