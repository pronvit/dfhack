#include "Core.h"
#include "DataDefs.h"
#include "Console.h"
#include "PluginManager.h"
#include "VTableInterpose.h"

#include "uicommon.h"
#include "modules/Gui.h"
#include "modules/Job.h"
#include "modules/Maps.h"
#include "modules/Screen.h"
#include "modules/World.h"

#include "df/building_nest_boxst.h"
#include "df/item.h"
#include "df/item_eggst.h"
#include "df/job.h"
#include "df/viewscreen_dwarfmodest.h"
#include "df/unit.h"

#define CONFIG_KEY "nestbox-guard/config"
#define TOGGLE_KEY df::interface_key::CUSTOM_E

using namespace DFHack;

using df::global::ui;
using df::global::world;

DFHACK_PLUGIN("nestbox-guard");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);

static void load_config();
static void save_config();
static void cancel_job(df::job* job);

static df::building_nest_boxst* getNestbox (const int32_t id)
{
    df::building* b = df::building::find(id);
    return virtual_cast<df::building_nest_boxst>(b);
}

static void forbid_egg (df::item_eggst* egg, bool forbid_state)
{
    if (forbid_state)
    {
        egg->flags.bits.forbid = true;
        egg->flags.bits.in_job = false;
        df::job_list_link* job_link = world->job_list.next;
        while (job_link != NULL)
        {
            df::job* job = job_link->item;
            for (auto iter = job->items.begin(); iter != job->items.end(); ++iter)
            {
                if ((*iter)->item->id == egg->id)
                {
                    cancel_job(job);
                }
            }
            job_link = job_link->next;
        }
    }
    else
    {
        egg->flags.bits.forbid = false;
    }
}

// Adapted from diggingInvaders
static void cancel_job (df::job* job) {
    if (job == NULL) {
        return;
    }

    df::unit* unit = Job::getWorker(job);
    if (unit)
        unit->job.current_job = NULL;
    if (job->list_link->prev != NULL)
        job->list_link->prev->next = job->list_link->next;
    if (job->list_link->next != NULL)
        job->list_link->next->prev = job->list_link->prev;
}

// Adapted from autotrade's WatchedBurrows
class Nestbox
{
public:
    int32_t id;
    df::building_nest_boxst* nestbox;
    bool isValid() { return nestbox != NULL && id == nestbox->id; }
    Nestbox(int32_t id) : id(id)
    {
        nestbox = getNestbox(id);
    }
    Nestbox(df::building_nest_boxst* nestbox) : nestbox(nestbox)
    {
        id = nestbox->id;
    }
};

class NestboxList
{
public:
    std::string serialize()
    {
        validate();
        std::stringstream id_stream;
        for (auto iter = nestboxes.begin(); iter != nestboxes.end(); ++iter)
        {
            if (iter != nestboxes.begin())
                id_stream << " ";
            id_stream << iter->id;
        }
        return id_stream.str();
    }

    void clear()
    {
        nestboxes.clear();
    }

    void add (df::building_nest_boxst* nestbox)
    {
        if (!nestbox)
            return;
        if (contains(nestbox))
            return;
        Nestbox nb(nestbox);
        nestboxes.push_back(nb);
    }

    void add (const int32_t id)
    {
        add(getNestbox(id));
    }

    void add (const std::string ids)
    {
        std::istringstream iss(ids);
        int id;
        while (iss >> id)
        {
            add(id);
        }
    }

    void remove (const df::building_nest_boxst* nestbox)
    {
        for (auto iter = nestboxes.begin(); iter != nestboxes.end(); )
        {
            if (iter->nestbox == nestbox)
                iter = nestboxes.erase(iter);
            else
                ++iter;
        }
    }

    void remove (const int32_t id)
    {
        remove(getNestbox(id));
    }

    bool contains (const df::building_nest_boxst *nestbox)
    {
        validate();
        for (auto iter = nestboxes.begin(); iter != nestboxes.end(); ++iter)
        {
            if (iter->nestbox == nestbox)
                return true;
        }

        return false;
    }

    std::vector<Nestbox>* get_nestboxes()
    {
        validate();
        return &nestboxes;
    }

private:
    void validate()
    {
        for (auto iter = nestboxes.begin(); iter != nestboxes.end(); )
        {
            if (!getNestbox(iter->id))
                iter = nestboxes.erase(iter);
            else
                ++iter;
        }
    }

    std::vector<Nestbox> nestboxes;
};

static NestboxList nestbox_list;
static PersistentDataItem persistent_config;

struct nestbox_guard_dwarfmode_hook : df::viewscreen_dwarfmodest
{
    typedef df::viewscreen_dwarfmodest interpose_base;

    df::building_nest_boxst* get_cur_nestbox()
    {
        return virtual_cast<df::building_nest_boxst>(world->selected_building);
    }

    bool is_valid_context()
    {
        return (ui->main.mode == df::ui_sidebar_mode::QueryBuilding ||
                ui->main.mode == df::ui_sidebar_mode::BuildingItems) &&
                get_cur_nestbox();
    }

    DEFINE_VMETHOD_INTERPOSE(void, feed, (std::set<df::interface_key>* input))
    {
        if (input->count(TOGGLE_KEY))
        {
            input->erase(TOGGLE_KEY);
            df::building_nest_boxst* cur_nestbox = get_cur_nestbox();
            if (nestbox_list.contains(cur_nestbox))
                nestbox_list.remove(cur_nestbox);
            else
                nestbox_list.add(cur_nestbox);
            save_config();
        }
        INTERPOSE_NEXT(feed)(input);
    }

    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        INTERPOSE_NEXT(render)();
        if (is_valid_context())
        {
            df::building_nest_boxst* cur_nestbox = get_cur_nestbox();
            auto dims = Gui::getDwarfmodeViewDims();
            int x = dims.menu_x1 + 1;
            int y = (ui->main.mode == df::ui_sidebar_mode::QueryBuilding)
                ? dims.y2 - 3
                : 18;
            OutputString(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(TOGGLE_KEY));
            OutputString(COLOR_WHITE, x, y, ": Auto-forbid eggs: ");
            bool active = nestbox_list.contains(cur_nestbox);
            OutputString(active ? COLOR_GREEN : COLOR_RED,
                         x, y, active ? "On" : "Off");
        }
    }
};
IMPLEMENT_VMETHOD_INTERPOSE(nestbox_guard_dwarfmode_hook, feed);
IMPLEMENT_VMETHOD_INTERPOSE(nestbox_guard_dwarfmode_hook, render);

static void load_config()
{
    nestbox_list.clear();
    persistent_config = World::GetPersistentData(CONFIG_KEY);
    if (persistent_config.isValid())
    {
        nestbox_list.add(persistent_config.val());
    }
    else
    {
        persistent_config = World::AddPersistentData(CONFIG_KEY);
        if (persistent_config.isValid())
            save_config();
    }
}

static void save_config()
{
    if (!persistent_config.isValid())
        return;
    persistent_config.val() = nestbox_list.serialize();
}

static void scan_nestboxes()
{
    std::vector<Nestbox>* nestboxes = nestbox_list.get_nestboxes();
    for (auto nb_iter = nestboxes->begin(); nb_iter != nestboxes->end(); ++nb_iter)
    {
        Nestbox nb = *nb_iter;
        if (!nb.isValid())
            continue;
        for (auto item_iter = nb.nestbox->contained_items.begin();
             item_iter != nb.nestbox->contained_items.end(); ++item_iter)
        {
            df::item_eggst* egg = virtual_cast<df::item_eggst>((*item_iter)->item);
            if (!egg)
                continue;
            forbid_egg(egg, egg->egg_flags.bits.fertile);
        }
    }
}

DFhackCExport command_result plugin_init (color_ostream& out, std::vector<PluginCommand>& commands)
{
#define require_global(name) \
    if (df::global::name == NULL) { \
        out.printerr("Global not available: %s\n", #name); \
        return CR_FAILURE; \
    }
    require_global(ui);
    require_global(world);
#undef require_global
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown (color_ostream& out)
{
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream& out, bool enable)
{
    if (enable != is_enabled)
    {
        if (!INTERPOSE_HOOK(nestbox_guard_dwarfmode_hook, render).apply(enable) ||
            !INTERPOSE_HOOK(nestbox_guard_dwarfmode_hook, feed).apply(enable))
        {
            out.printerr("nestbox-guard: Failed to enable dwarfmode hooks!\n");
            return CR_FAILURE;
        }
        is_enabled = enable;
        if (Maps::IsValid())
        {
            if (is_enabled)
                load_config();
            else
                save_config();
        }
    }
    return CR_OK;
}

DFhackCExport command_result plugin_onstatechange (color_ostream& out, state_change_event event)
{
    if (!is_enabled)
        return CR_OK;
    switch (event)
    {
        case SC_MAP_LOADED:
            load_config();
            break;
        default:
            break;
    }
    return CR_OK;
}

DFhackCExport command_result plugin_onupdate (color_ostream& out)
{
    if (!is_enabled || !Maps::IsValid())
        return CR_OK;
    if (DFHack::World::ReadPauseState())
        return CR_OK;
    static decltype(world->frame_counter) last_frame_count = 0;

    if (world->frame_counter - last_frame_count < 25)
        return CR_OK;
    last_frame_count = world->frame_counter;

    scan_nestboxes();
    return CR_OK;
}
