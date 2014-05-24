#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "Export.h"
#include "PluginManager.h"

#include <regex>
#include <set>

#include <modules/Gui.h>
#include <modules/Screen.h>
#include <VTableInterpose.h>

#include "ColorText.h"
#include "df/enabler.h"
#include "df/graphic.h"
#include "df/viewscreen_titlest.h"
#include "df/viewscreen_loadgamest.h"

#define LOAD_LIST_MAX_X 77
#define DF_CHAR(char) df::interface_key::STRING_A##char

using namespace DFHack;

using df::global::enabler;
using df::global::gps;

typedef df::viewscreen_loadgamest::T_saves T_saves;

class viewscreen_load_screen;
class viewscreen_load_options;

void OutputString (int8_t fg, int x, int y, const std::string text);
void OutputStringX (int8_t fg, int &x, int y, const std::string text);
void OutputStringCenter (int8_t fg, int y, const std::string text);
std::string mode_string (df::game_type gametype);
command_result cmd_load_screen (color_ostream &out, std::vector <std::string> & parameters);

enum loadgame_flags
{
    LG_NONE = 0,
    LG_HIDE_BACKUPS = 1,
    LG_COLOR_MODE = 2,
};
#define LG_DEFAULT_FLAGS (LG_HIDE_BACKUPS | LG_COLOR_MODE)

inline loadgame_flags operator| (loadgame_flags a, loadgame_flags b)
{
    return static_cast<loadgame_flags>(static_cast<int>(a) | static_cast<int>(b));
}
inline loadgame_flags operator& (loadgame_flags a, loadgame_flags b)
{
    return static_cast<loadgame_flags>(static_cast<int>(a) & static_cast<int>(b));
}
inline loadgame_flags operator^ (loadgame_flags a, loadgame_flags b)
{
    return static_cast<loadgame_flags>(static_cast<int>(a) ^ static_cast<int>(b));
}

class SaveGame
{
public:
    SaveGame (T_saves save);
    T_saves save;
    df::game_type game_type;
    std::string fort_name;
    std::string world_name;
    int32_t year;
    std::string folder_name;
    std::string base_folder_name;
    bool is_backup;
};

class viewscreen_load_screen : public dfhack_viewscreen
{
public:
    loadgame_flags flags;

    viewscreen_load_screen ();
    ~viewscreen_load_screen () { };

    void feed (std::set<df::interface_key> *input);
    void render ();
    void help () { };
    std::string getFocusString () { return "loadscreen"; };

    df::viewscreen_loadgamest* parent_screen ();
    void dismiss ();
    bool select_game (std::string folder);
    bool load_game (std::string folder);
protected:
    int sel_idx;
    int sel_offset;
    std::vector<SaveGame*> save_folders;
    void init_save_folders ();
    void show_options ();
    std::vector<SaveGame*> get_saves ();
    bool mouse_select_save (int x, int y);
};

class viewscreen_load_options : public dfhack_viewscreen
{
public:
    void feed (std::set<df::interface_key> *input);
    void render ();
    void help () { };
    std::string getFocusString () { return "loadscreen/options"; };
    viewscreen_load_options (viewscreen_load_screen * parent, SaveGame * save);
    ~viewscreen_load_options () { };

    bool rename_folder (std::string new_folder);
    int getWidth () { return width; };
    int getHeight () { return height; };
protected:
    viewscreen_load_screen * parent;
    SaveGame * save;
    int width;
    int height;
    void do_load ();
    bool load_flag;
};

class entry_dialog : public dfhack_viewscreen
{
public:
    void feed (std::set<df::interface_key> *input);
    void render ();
    void help () { };
    virtual std::string getFocusString () = 0;
    entry_dialog (dfhack_viewscreen * parent);
    ~entry_dialog () { };

    std::string getEntry () { return this->entry; };
    virtual std::string getTitle () = 0;
    virtual std::string getPrompt () = 0;
protected:
    dfhack_viewscreen * parent;
    virtual bool validate (char input) = 0;
    virtual void close () { Screen::dismiss(this); };
    virtual bool submit () = 0;
    std::string entry;
    int width;
    int height;
};

class viewscreen_rename_dialog : public entry_dialog
{
public:
    viewscreen_rename_dialog (viewscreen_load_options * parent, SaveGame * save);
    ~viewscreen_rename_dialog () { };
    std::string getFocusString () { return "loadscreen/rename"; };
    std::string getTitle () { return "Renaming " + this->save->folder_name; };
    std::string getPrompt () { return "New folder name:"; };
    bool validate (char ch);
protected:
    viewscreen_load_options * parent;
    SaveGame * save;
    bool submit ();
};

//class viewscreen_rename_dialog : public dfhack_viewscreen
//{
//public:
//    void feed (std::set<df::interface_key> *input);
//    void render ();
//    void help () { };
//    std::string getFocusString () { return "loadscreen/rename"; };
//    viewscreen_rename_dialog (viewscreen_load_options * parent, SaveGame * save);
//    ~viewscreen_rename_dialog () { };
//protected:
//    viewscreen_load_options * parent;
//    SaveGame * save;
//    std::string entry;
//};

void OutputString (int8_t fg, int x, int y, const std::string text)
{
    Screen::paintString(Screen::Pen(' ', fg, COLOR_BLACK), x, y, text);
}
void OutputStringX (int8_t fg, int &x, int y, const std::string text)
{
    OutputString(fg, x, y, text);
    x += text.length();
}
void OutputStringCenter (int8_t fg, int y, const std::string text)
{
    int x = Screen::getWindowSize().x / 2 - text.length() / 2;
    OutputString(fg, x, y, text);
}
void OutputCheck (int x, int y, bool value)
{
    color_value fg = (value) ? COLOR_LIGHTGREEN : COLOR_LIGHTRED;
    std::string text = (value) ? "\373" : " ";
    OutputString(fg, x, y, text);
}

std::string mode_string (df::game_type gametype)
{
    switch (gametype)
    {
        case df::game_type::DWARF_MAIN:
            return "Fortress mode";
        case df::game_type::DWARF_RECLAIM:
            return "Reclaim fortress mode";
        case df::game_type::ADVENTURE_MAIN:
            return "Adventure mode";
        default:
            return "";
    }
}

SaveGame::SaveGame (T_saves save):
    save(save),
    is_backup(false)
{
    this->game_type = save.game_type;
    this->fort_name = save.fort_name;
    this->world_name = save.world_name;
    this->year = save.year;
    this->folder_name = this->base_folder_name = save.folder_name;
    int pos = 0;
    if ((pos = folder_name.find("-")) != std::string::npos)
    {
        this->base_folder_name = this->folder_name.substr(0, pos);
        this->is_backup = true;
    }
}

viewscreen_load_screen::viewscreen_load_screen ():
    sel_idx(0),
    sel_offset(0),
    flags(LG_DEFAULT_FLAGS)
{ }

df::viewscreen_loadgamest* viewscreen_load_screen::parent_screen ()
{
    df::viewscreen * top = this->parent;
    VIRTUAL_CAST_VAR(parent_screen, df::viewscreen_loadgamest, top);
    return parent_screen;
}

void viewscreen_load_screen::init_save_folders ()
{
    std::vector<T_saves*> saves = parent_screen()->saves;
    for (auto iter = saves.begin(); iter != saves.end(); iter++)
    {
        T_saves * t_save = *iter;
        SaveGame * save = new SaveGame(*t_save);
        save_folders.push_back(save);
    }
}

std::vector<SaveGame*> viewscreen_load_screen::get_saves ()
{
    std::vector<SaveGame*> saves;
    for (auto iter = this->save_folders.begin(); iter != this->save_folders.end(); iter++)
    {
        SaveGame * save = *iter;
        if (save->is_backup && this->flags & LG_HIDE_BACKUPS) continue;
        saves.push_back(save);
    }
    return saves;
}

bool viewscreen_load_screen::mouse_select_save (int x, int y)
{
    if (x < 2 || x > LOAD_LIST_MAX_X)
        return false;
    int index = (y / 2) - 1 + sel_offset;
    if (index < 0 || index >= this->get_saves().size())
        return false;
    sel_idx = index;
    return true;
}

void viewscreen_load_screen::show_options ()
{
    Screen::show(new viewscreen_load_options(this, this->get_saves()[sel_idx]));
}

void viewscreen_load_screen::feed (std::set<df::interface_key> *input)
{
    auto saves = this->get_saves();
    if (input->count(df::interface_key::LEAVESCREEN))
    {
        this->dismiss();
        return;
    }
    else if (input->count(df::interface_key::CURSOR_DOWN))
    {
        sel_idx++;
        if (sel_idx >= saves.size())
            sel_idx = 0;
    }
    else if (input->count(df::interface_key::CURSOR_UP))
    {
        sel_idx--;
        if (sel_idx < 0)
            sel_idx = saves.size() - 1;
    }
    else if (input->count(df::interface_key::SELECT))
    {
        show_options();
    }
    else if (input->count(df::interface_key::CUSTOM_B))
    {
        this->flags = this->flags ^ LG_HIDE_BACKUPS;
    }
    else if (input->count(df::interface_key::CUSTOM_C))
    {
        this->flags = this->flags ^ LG_COLOR_MODE;
    }

    if (enabler->tracking_on && enabler->mouse_lbut)
    {
        if (mouse_select_save(gps->mouse_x, gps->mouse_y))
            show_options();
    }
}

void viewscreen_load_screen::render ()
{
    if (!save_folders.size())
        init_save_folders();
    int x, y;
    auto dim = Screen::getWindowSize();
    df::viewscreen_loadgamest* parent = parent_screen();
    if (!parent)
        return;
    Screen::clear();
    std::string title = "Load game (DFHack)";
    Screen::paintString(Screen::Pen(' ', COLOR_WHITE, COLOR_BLACK),
                        (dim.x / 2) - (title.length() / 2),
                        0,
                        title);
    //auto games = save_folders;
    sel_idx = std::min(sel_idx, (int)this->get_saves().size() - 1);
    sel_offset = std::min(sel_offset, (int)this->get_saves().size() - 1);
    auto games = this->get_saves();
    int max_rows = (dim.y / 2) - 1;
    int row = 2, i = this->sel_offset;
    x = 1; y = 0;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_B));
    OutputStringX(COLOR_WHITE, x, y, ": Hide backups [ ]");
    OutputCheck(x - 2, y, this->flags & LG_HIDE_BACKUPS);
    x = 1; y++;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_C));
    OutputStringX(COLOR_WHITE, x, y, ": Color [ ]");
    OutputCheck(x - 2, y, this->flags & LG_COLOR_MODE);
    for (auto iter = games.begin() + sel_offset; iter != games.end() && row + 2 < dim.y; iter++, row += 2, i++)
    {
        SaveGame * save = *iter;
        /* Foreground colors:
         * white: Fortress mode
         * cyan: Adventure mode
         * magenta: Reclaim fortress mode
         * red: Backup
         */
        color_value fg = COLOR_GREY;
        if (this->flags & LG_COLOR_MODE)
        {
            if (save->is_backup) fg = COLOR_RED;
            else if (save->game_type == df::game_type::ADVENTURE_MAIN) fg = COLOR_CYAN;
            else if (save->game_type == df::game_type::DWARF_RECLAIM) fg = COLOR_MAGENTA;
        }
        if (i == sel_idx) fg = (color_value)(fg + 8);
        color_value bg = (i == sel_idx) ? COLOR_BLUE : COLOR_BLACK;

        auto pen = Screen::Pen(' ', fg, bg);
        Screen::fillRect(pen, 2, row, LOAD_LIST_MAX_X, row + 1);
        Screen::paintString(pen, 2, row, save->fort_name + " - " + mode_string(save->game_type));
        Screen::paintString(pen, 3, row + 1, "Folder: " + save->folder_name);
        std::string world_name = save->world_name;
        Screen::paintString(pen, LOAD_LIST_MAX_X - world_name.length() + 1, row, world_name);
        std::string year = std::to_string(save->year);
        Screen::paintString(pen, LOAD_LIST_MAX_X - year.length() + 1, row + 1, year);
    }
    if (sel_offset > 0)
        OutputStringCenter(COLOR_LIGHTCYAN, 1, "\030 More \030");
    if (sel_offset < games.size() - max_rows && max_rows < games.size())
        OutputStringCenter(COLOR_LIGHTCYAN, dim.y - 1, "\031 More \031");
    if (enabler->tracking_on && gps->mouse_x != -1 && gps->mouse_y != -1 &&
            Gui::getCurFocus() == "dfhack/loadscreen")
    {
        //mouse_select_save(gps->mouse_x, gps->mouse_y);
        bool scroll = false;
        if (gps->mouse_y == dim.y - 1)
        {
            this->sel_offset++;
            this->sel_offset = std::min((int)games.size() - max_rows + 1, this->sel_offset);
            scroll = true;
        }
        else if (gps->mouse_y < 2 && sel_offset > 0)
        {
            this->sel_offset--;
            this->sel_offset = std::max(0, this->sel_offset);
            scroll = true;
        }
        if (scroll)
        {
            this->sel_offset = std::max(0, std::min((int)games.size(), sel_offset));
            this->sel_idx = std::max(this->sel_offset, std::min(this->sel_offset + max_rows - 1, this->sel_idx));
        }
    }
}

bool viewscreen_load_screen::select_game (std::string folder)
{
    int i = 0;
    auto games = this->get_saves();
    for (auto iter = games.begin(); iter != games.end(); iter++, i++)
    {
        SaveGame * save = *iter;
        if (folder == save->folder_name)
        {
            this->sel_idx = i;
            this->show_options();
            return true;
        }
    }
    return false;
}

bool viewscreen_load_screen::load_game (std::string folder)
{
    df::viewscreen_loadgamest* parent = parent_screen();
    parent->sel_idx = 0;
    parent->saves[0]->folder_name = folder;
    parent->loading = 1;
    parent->logic();
    return true;
}

void viewscreen_load_screen::dismiss ()
{
    Screen::dismiss(this);
    Screen::dismiss(this->parent);
}

viewscreen_load_options::viewscreen_load_options (viewscreen_load_screen * parent, SaveGame * save):
    parent(parent),
    save(save),
    width(42),
    load_flag(false)
{ }

void viewscreen_load_options::feed (std::set<df::interface_key> *input)
{
    if (input->count(df::interface_key::LEAVESCREEN))
    {
        Screen::dismiss(this);
    }
    else if (input->count(df::interface_key::SELECT))
    {
        this->load_flag = true;
    }
    else if (input->count(df::interface_key::CUSTOM_U))
    {
        if (this->save->is_backup)
        {
            Screen::dismiss(this);
            this->parent->select_game(this->save->base_folder_name);
        }
    }
    else if (input->count(df::interface_key::CUSTOM_R))
    {
        viewscreen_rename_dialog * dialog = new viewscreen_rename_dialog(this, this->save);
        Screen::show(dialog);
    }
}

void viewscreen_load_options::render ()
{
    if (load_flag)
    {
        Screen::clear();
        OutputString(COLOR_WHITE, 2, 2, "Loading game...");
        do_load();
        return;
    }

    int x, y;
    std::string play_now_label =  ": Play now";
    std::string is_backup_warning, is_backup_warning2;

    height = 8;
    if (this->save->is_backup)
    {
        height += 3;
        is_backup_warning = "Warning: This is a backup of \"" + this->save->base_folder_name + "\"";
        is_backup_warning2 = "You're probably looking for the original save";
        play_now_label = ": Play anyway";
    }
    width = std::max(width, std::max((int)is_backup_warning.length(), (int)is_backup_warning2.length()) + 4);

    parent->render();
    auto dim = Screen::getWindowSize();
    int min_x = (dim.x - width) / 2,
        max_x = (dim.x + width) / 2,
        min_y = (dim.y - height) / 2,
        max_y = (dim.y + height) / 2;
    y = max_y - (height - 2);
    Screen::fillRect(Screen::Pen(' ', COLOR_BLACK, COLOR_DARKGREY), min_x, min_y, max_x, max_y);  // border
    Screen::fillRect(Screen::Pen(' ', COLOR_BLACK, COLOR_BLACK),
                     min_x + 1, min_y + 1, max_x - 1, max_y - 1);  // content
    std::string title = " Load game: " + this->save->folder_name + " ";
    Screen::paintString(Screen::Pen(' ', COLOR_BLACK, COLOR_GREY),
                        dim.x / 2 - title.length() / 2, min_y, title);
    if (this->save->is_backup)
    {
        x = min_x + 2; y = min_y + 2;
        OutputString(COLOR_LIGHTMAGENTA, x, y, is_backup_warning);
        y++;
        OutputString(COLOR_LIGHTMAGENTA, x, y, is_backup_warning2);
        y++;
        OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_U));
        OutputStringX(COLOR_LIGHTGREEN, x, y, ": Use " + this->save->base_folder_name);
        y++;
    }
    x = min_x + 2;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_B));
    OutputStringX(COLOR_WHITE, x, y, ": Load from backup");
    x = min_x + 2; y++;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_D));
    OutputStringX(COLOR_WHITE, x, y, ": Delete old backups");
    x = min_x + 2; y++;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_R));
    OutputStringX(COLOR_WHITE, x, y, ": Rename");

    x = min_x + 2; y++;
    x = max_x - 1 - play_now_label.length() - Screen::getKeyDisplay(df::interface_key::SELECT).length();
    y++;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::SELECT));
    OutputStringX(COLOR_WHITE, x, y, play_now_label);
    x = min_x + 2;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::LEAVESCREEN));
    OutputStringX(COLOR_WHITE, x, y, ": Back");
}

void viewscreen_load_options::do_load ()
{
    this->parent->load_game(save->folder_name);
    Screen::dismiss(this);
    this->parent->dismiss();
}

bool viewscreen_load_options::rename_folder (std::string new_folder)
{
    int result = rename(
        ((std::string)"data/save/" + this->save->folder_name).c_str(),
        ((std::string)"data/save/" + new_folder).c_str()
    );
    if (result)
        return false;
    this->save->folder_name = new_folder;
    this->save->base_folder_name = new_folder;
    this->save->is_backup = false;
    return true;
}

entry_dialog::entry_dialog (dfhack_viewscreen * parent):
    parent(parent)
{ }

void entry_dialog::render ()
{
    parent->render();
    auto dim = Screen::getWindowSize();
    int width = this->width,
        height = this->height,
        min_x = (dim.x - width) / 2,
        max_x = (dim.x + width) / 2,
        min_y = (dim.y - height) / 2,
        max_y = (dim.y + height) / 2;
    Screen::fillRect(Screen::Pen(' ', COLOR_BLACK, COLOR_DARKGREY), min_x, min_y, max_x, max_y);
    Screen::fillRect(Screen::Pen(' ', COLOR_BLACK, COLOR_BLACK), min_x + 1, min_y + 1, max_x - 1, max_y - 1);
    std::string title = " " + this->getTitle() + " ";
    Screen::paintString(Screen::Pen(' ', COLOR_BLACK, COLOR_GREY), (dim.x - title.length()) / 2, min_y, title);
    int x = min_x + 2,
        y = min_y + 2;
    std::string prompt = this->getPrompt();
    OutputString(COLOR_WHITE, x, y, prompt);
    y += 2;
    color_value text_fg = (this->entry.length() >= width - 4) ? COLOR_LIGHTRED : COLOR_WHITE;
    OutputStringX(text_fg, x, y, this->entry);
    OutputStringX(COLOR_LIGHTCYAN, x, y, "_");
    x = min_x + 2; y = max_y - 2;
    OutputStringX(COLOR_LIGHTRED, x, y, Screen::getKeyDisplay(df::interface_key::CUSTOM_ALT_C));
    OutputStringX(COLOR_WHITE, x, y, ": Clear");
}

void entry_dialog::feed (std::set<df::interface_key> *input)
{
    int ascii_offset = df::interface_key::STRING_A048 - '0';
    for (auto iter = input->begin(); iter != input->end(); iter++)
    {
        df::interface_key key = *iter;
        if (key == df::interface_key::LEAVESCREEN)
        {
            this->close();
        }
        else if (key == df::interface_key::SELECT)
        {
            if (this->submit())
            {
                this->close();
            }
        }
        else if (key == DF_CHAR(000) && this->entry.length() > 0)
        {
            this->entry = this->entry.erase(this->entry.length() - 1);
        }
        else if (key >= DF_CHAR(032) && key <= DF_CHAR(126))
        {
            char ch = key - ascii_offset;
            if (this->validate(ch))
            {
                this->entry += ch;
            }
        }
        else if (key == df::interface_key::CUSTOM_ALT_C)
        {
            this->entry = "";
        }
    }
}

viewscreen_rename_dialog::viewscreen_rename_dialog (viewscreen_load_options * parent, SaveGame * save):
    entry_dialog(parent),
    parent(parent),
    save(save)
{
    this->entry = save->folder_name;
    this->width = parent->getWidth();
    this->height = parent->getHeight();
}

bool viewscreen_rename_dialog::submit ()
{
    if (this->entry.length() > 0)
    {
        if (this->parent->rename_folder(this->entry))
            return true;
    }
    return false;
}

bool viewscreen_rename_dialog::validate (char ch)
{
    if (ch == '-' ||
        ch == '_' ||
        (ch >= '0' && ch <= '9') ||
        (ch >= 'A' && ch <= 'Z') ||
        (ch >= 'a' && ch <= 'z')
    )
    {
        return true;
    }
    return false;
}

struct loadgame_hooks : df::viewscreen_loadgamest
{
    typedef df::viewscreen_loadgamest interpose_base;
    DEFINE_VMETHOD_INTERPOSE(void, render, ())
    {
        Screen::show(new viewscreen_load_screen);
    }
};

IMPLEMENT_VMETHOD_INTERPOSE(loadgame_hooks, render);

DFHACK_PLUGIN("load-screen");
DFHACK_PLUGIN_IS_ENABLED(is_enabled);

DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
    return CR_OK;
}

DFhackCExport command_result plugin_enable (color_ostream &out, bool enable)
{
    if (!gps)
        return CR_FAILURE;
    if (enable != is_enabled)
    {
        if (!INTERPOSE_HOOK(loadgame_hooks, render).apply(enable))
            return CR_FAILURE;
        is_enabled = enable;
    }
    return CR_OK;
}

