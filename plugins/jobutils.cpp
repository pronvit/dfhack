#include "Core.h"
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>
#include <MiscUtils.h>

#include <modules/Materials.h>

#include <DataDefs.h>
#include <df/world.h>
#include <df/ui.h>
#include <df/ui_build_selector.h>
#include <df/ui_build_item_req.h>
#include <df/build_req_choice_genst.h>
#include <df/build_req_choice_specst.h>
#include <df/building_workshopst.h>
#include <df/building_furnacest.h>
#include <df/job.h>
#include <df/job_item.h>
#include <df/item.h>

using std::vector;
using std::string;
using std::endl;
using namespace DFHack;
using namespace df::enums;

using df::global::world;
using df::global::ui;
using df::global::ui_build_selector;
using df::global::ui_workshop_job_cursor;

static bool job_material_hotkey(Core *c, df::viewscreen *top);
static command_result job_material(Core *c, vector <string> & parameters);

DFhackCExport const char * plugin_name ( void )
{
    return "jobutils";
}

DFhackCExport command_result plugin_init (Core *c, std::vector <PluginCommand> &commands)
{
    commands.clear();
    if (world && ui && (ui_workshop_job_cursor || ui_build_selector)) {
        commands.push_back(
            PluginCommand(
                "job-material", "Alter the material of the selected job.",
                job_material, job_material_hotkey,
                "  job-material <inorganic-token>\n"
                "Intended to be used as a keybinding:\n"
                "  - In 'q' mode, when a job is highlighted within a workshop\n"
                "    or furnace, changes the material of the job.\n"
                "  - In 'b' mode, during selection of building components\n"
                "    positions the cursor over the first available choice\n"
                "    with the matching material.\n"
            )
        );
    }
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( Core * c )
{
    return CR_OK;
}

static bool job_material_hotkey(Core *c, df::viewscreen *top)
{
    using namespace ui_sidebar_mode;

    if (!dwarfmode_hotkey(c,top))
        return false;

    switch (ui->main.mode) {
    case QueryBuilding:
        {
            if (!ui_workshop_job_cursor) // allow missing
                return false;

            df::building *selected = world->selected_building;
            if (!virtual_cast<df::building_workshopst>(selected) &&
                !virtual_cast<df::building_furnacest>(selected))
                return false;

            // No jobs?
            if (selected->jobs.empty() ||
                selected->jobs[0]->job_id == job_type::DestroyBuilding)
                return false;

            // Add job gui activated?
            if (df::global::ui_workshop_in_add && // allow missing
                *df::global::ui_workshop_in_add)
                return false;

            return true;
        };
    case Build:
        {
            if (!ui_build_selector) // allow missing
                return false;

            // Not selecting, or no choices?
            if (ui_build_selector->building_type < 0 ||
                ui_build_selector->stage != 2 ||
                ui_build_selector->choices.empty())
                return false;

            return true;
        };
    default:
        return false;
    }
}

static command_result job_material_in_job(Core *c, MaterialInfo &new_mat)
{
    df::building *selected = world->selected_building;
    int idx = *ui_workshop_job_cursor;

    if (idx < 0 || idx >= selected->jobs.size())
    {
        c->con.printerr("Invalid job cursor index: %d\n", idx);
        return CR_FAILURE;
    }

    df::job *job = selected->jobs[idx];

    MaterialInfo cur_mat(job->matType, job->matIndex);

    if (!cur_mat.isValid() || cur_mat.type != 0)
    {
        c->con.printerr("Current job material isn't inorganic: %s\n",
                        cur_mat.toString().c_str());
        return CR_FAILURE;
    }

    df::craft_material_class old_class = cur_mat.getCraftClass();
    if (old_class == craft_material_class::None)
    {
        c->con.printerr("Unexpected current material type: %s\n",
                        cur_mat.toString().c_str());
        return CR_FAILURE;
    }
    if (new_mat.getCraftClass() != old_class)
    {
        c->con.printerr("New material %s does not satisfy requirement: %s\n",
                        new_mat.toString().c_str(), ENUM_KEY_STR(craft_material_class, old_class));
        return CR_FAILURE;
    }

    for (unsigned i = 0; i < job->job_items.size(); i++)
    {
        df::job_item *item = job->job_items[i];
        MaterialInfo item_mat(item->matType, item->matIndex);

        if (item_mat != cur_mat)
        {
            c->con.printerr("Job item %d has different material: %s\n",
                            i, item_mat.toString().c_str());
            return CR_FAILURE;
        }
    }

    // Apply the substitution
    job->matType = new_mat.type;
    job->matIndex = new_mat.index;

    for (unsigned i = 0; i < job->job_items.size(); i++)
    {
        df::job_item *item = job->job_items[i];
        item->matType = new_mat.type;
        item->matIndex = new_mat.index;
    }

    c->con << "Applied material '" << new_mat.toString()
           << "' to job " << ENUM_KEY_STR(job_type,job->job_id) << endl;
    return CR_OK;
}

static bool build_choice_matches(df::ui_build_item_req *req, df::build_req_choicest *choice,
                                 MaterialInfo &new_mat)
{
    if (VIRTUAL_CAST_VAR(gen, df::build_req_choice_genst, choice))
    {
        if (gen->matType == new_mat.type &&
            gen->matIndex == new_mat.index &&
            gen->used_count < gen->candidates.size())
        {
            return true;
        }
    }
    else if (VIRTUAL_CAST_VAR(spec, df::build_req_choice_specst, choice))
    {
        if (spec->candidate &&
            spec->candidate->getActualMaterial() == new_mat.type &&
            spec->candidate->getActualMaterialIndex() == new_mat.index &&
            !req->candidate_selected[spec->candidate_id])
        {
            return true;
        }
    }

    return false;
}

static command_result job_material_in_build(Core *c, MaterialInfo &new_mat)
{
    df::ui_build_item_req *req = ui_build_selector->requirements[ui_build_selector->req_index];

    for (unsigned i = 0; i < ui_build_selector->choices.size(); i++)
    {
        if (build_choice_matches(req, ui_build_selector->choices[i], new_mat))
        {
            ui_build_selector->sel_index = i;
            return CR_OK;
        }
    }

    c->con.printerr("Could not find material in list: %s\n", new_mat.toString().c_str());
    return CR_FAILURE;
}

static command_result job_material(Core * c, vector <string> & parameters)
{
    // HOTKEY COMMAND: CORE ALREADY SUSPENDED

    MaterialInfo new_mat;
    if (parameters.size() == 1)
    {
        if (!new_mat.findInorganic(parameters[0])) {
            c->con.printerr("Could not find inorganic material: %s\n", parameters[0].c_str());
            return CR_WRONG_USAGE;
        }
    }
    else
        return CR_WRONG_USAGE;

    if (ui->main.mode == ui_sidebar_mode::QueryBuilding)
        return job_material_in_job(c, new_mat);
    if (ui->main.mode == ui_sidebar_mode::Build)
        return job_material_in_build(c, new_mat);

    return CR_WRONG_USAGE;
}
