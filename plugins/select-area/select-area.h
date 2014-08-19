#pragma once

using df::global::ui;

enum select_area_sidebar_state {
    SA_SELECT_BRUSH,
    SA_SELECT_POINT,
    SA_SELECT_ACTION_TYPE,
    SA_SELECT_DESIGNATE,
    SA_SELECT_BUILD,
};

struct menu_option {
    select_area_sidebar_state state;
    df::interface_key key;
    std::string title;
};

const menu_option menu_options[] = {
    {SA_SELECT_BRUSH, interface_key::CUSTOM_R, "Rectangle"},
    {SA_SELECT_BRUSH, interface_key::CUSTOM_C, "Circle"},
};

static bool clear_sidebar_menu()
{
    auto dims = Gui::getDwarfmodeViewDims();
    if (dims.menu_x1 != -1)
    {
        Screen::fillRect(Screen::Pen(' ', COLOR_BLACK), dims.menu_x1, dims.y1, dims.menu_x2, dims.y2);
        return true;
    }
    return false;
}

class select_area_sidebar : public dfhack_viewscreen
{
private:
    select_area_sidebar_state state;
    int menu_x_offset()
    {
        auto dims = Gui::getDwarfmodeViewDims();
        return dims.menu_x1;
    }
    void change_state (select_area_sidebar_state new_state);
    void display_menu (select_area_sidebar_state state);
public:
    select_area_sidebar()
    {
        change_state(SA_SELECT_BRUSH);
    }
    std::string getFocusString() { return "SelectArea"; }
    void render();
    void feed(std::set<df::interface_key> *input);
};
