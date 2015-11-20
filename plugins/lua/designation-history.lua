local _ENV = mkmodule('plugins.designation-history')

gui = require 'gui'
guidm = require 'gui.dwarfmode'

--
-- Create classes
--

HistScreen = defclass(HistScreen, guidm.MenuOverlay)
HistRows = defclass(HistRows)

--
-- HistRows
--

function HistRows:init()
    self:read_history()
    self.cursor = 1
    self.marked_rows = 0
end

function HistRows:read_history()
    -- reverse - list newest first
    local hist = get_history()
    self.history = {}
    for i, v in pairs(hist) do
        self.history[#hist - i + 1] = v
    end
end

function HistRows:has_rows()
    return #self.history > 0
end

function HistRows:max_page()
    return math.floor(#self.history / self.page_height + 1)
end

function HistRows:paginate(screen_rows)
    local min_page_size = 15 -- max rows that can be shown at 25 row resolution
    if screen_rows then self.screen_rows = screen_rows end
    self.page_height = math.max(self.screen_rows - 11, min_page_size)
    if self.cursor <= self.page_height then
        self.page = 1
    else
        self.page = math.floor((self.cursor -1) / self.page_height) + 1
    end
end

function HistRows:update_scroll(delta, page_scroll, opts)
    if not opts then opts = {} end
    if opts.wrap == nil then opts.wrap = true end
    local min = 1
    local max = #self.history
    if page_scroll then delta = delta * self.page_height end
    local index = self.cursor + delta

    if delta < 0 and index < min then
        if index <= min + delta and opts.wrap then
            index = max
        else
            index = min
        end
    elseif delta > 0 and index > max then
        if index >= max + delta and opts.wrap then
            index = min
        else
            index = max
        end
    end
    self.cursor = index
    self:paginate()
end

function HistRows:get_row(index)
    if not index then index = self.cursor end
    return self.history[index]
end

function HistRows:mark_row()
    if self.history[self.cursor].mark then
        self.history[self.cursor].mark = false
        self.marked_rows = self.marked_rows - 1
    else
        self.history[self.cursor].mark = true
        self.marked_rows = self.marked_rows + 1
    end
end

function HistRows:has_marked_rows()
    return self.marked_rows > 0
end

function HistRows:delete_history()
    self:delete_rows(1, #self.history)
end

function HistRows:delete_marked_rows()
    for i = 1, #self.history do
        if self.history[i].mark then
            self:delete_rows(i, i, {reread=false})
        end
    end
    self:read_history()
end

function HistRows:delete_row()
    self:delete_rows(self.cursor)
end

function HistRows:delete_rows(first, last, opts)
    if not opts then opts = {reread=true} end
    if not last then last = first end
    -- re-reverse the IDs for a ranged delete
    if first ~= last then first, last = last, first end
    remove_history(self.history[first].id, self.history[last].id)
    if opts.reread then self:read_history() end
    if self.cursor > #self.history then self.cursor = #self.history end
end

function HistRows:stage_marked(stage)
    for i = 1, #self.history do
        if self.history[i].mark then
            self:stage_row(stage, self.history[i])
        end
    end
end

function HistRows:stage_row(stage, item)
    if not item then item = self.history[self.cursor] end
    reset_stage(item.id, stage)
end

function HistRows:zoom_row()
    local item = self.history[self.cursor]
    local pos = {x = item.centerx, y = item.centery, z = item.centerz}
    df.global.cursor:assign(pos)
    dfhack.gui.revealInDwarfmodeMap(pos, true)
end

function HistRows:print_rows(print_func)
    local first = self.page * self.page_height - self.page_height + 1
    local last = math.min(self.page * self.page_height, #self.history)
    for i = first, last do
        print_func(i, self.history[i].mark, i == self.cursor and true or false,
        {self.history[i].dimx, self.history[i].dimy, self.history[i].dimz, self.history[i].desc})
    end
end

--
-- HistScreen
--

function HistScreen:init()
    self.super.init(self)
    self.rows = HistRows()
end

function HistScreen:onResize(screen_cols, screen_rows)
    -- Update pagination for size of active window
    self.super.onResize(self, screen_cols, screen_rows)
    self.rows:paginate(screen_rows)
end

function HistScreen:render_menu()
    self.p:seek(0, self.p.y2 - 6)
    self.p:key('SELECT'):string(': Mark, ')
    self.p:key('CUSTOM_D'):string(': Delete ' .. (self.rows:has_marked_rows() and 'marks' or 'row')):newline()
    self.p:key('CUSTOM_SHIFT_D'):string(': Delete all'):newline()
    if self.rows:has_marked_rows() then
        self.p:key('CUSTOM_SHIFT_U'):string(': Undo all marked'):newline()
        self.p:key('CUSTOM_SHIFT_R'):string(': Redo all marked'):newline()
    end
    self.p:key('CUSTOM_U'):string(': Undo, ')
    self.p:key('CUSTOM_R'):string(': Redo, ')
    self.p:key('CUSTOM_Z'):string(': Zoom')
    self.p:newline()
    self.p:key('SECONDSCROLL_DOWN'):key('SECONDSCROLL_UP'):key('SECONDSCROLL_PAGEDOWN'):key('SECONDSCROLL_PAGEUP')
    self.p:string(': Scroll')
end

function HistScreen:render_rows()
    self.p:string('  Page ' .. self.rows.page .. ' of ' .. self.rows:max_page(), {fg = COLOR_GREEN}):newline()
    self.rows:print_rows(
        function(idx, marked, is_cursor, tab)
            row_color = marked and COLOR_RED or COLOR_GREY
            row_color = is_cursor and row_color + 8 or row_color
            self.p:string(('%2i: (%ix%ix%i) %s'):format(idx, tab[1], tab[2], tab[3], tab[4]),
                {fg = row_color})
            self.p:newline()
        end
    )
end

function HistScreen:render(_p)
    self.super.render(self, _p)
    self.p = gui.Painter.new_wh(
        self.df_layout.menu.x1 + 1,
        self.df_layout.menu.y1,
        self.df_layout.menu.width - 2,
        self.df_layout.menu.height - 1
    )
    if not self.rows:has_rows() then
        self.p:seek(0, self.p.y1)
        self.p:string('History empty', {fg = COLOR_LIGHTRED}):newline()
        self.p:pen(COLOR_DARKGREY)  -- render menu as 'greyed out'
        self.p:key_pen(COLOR_RED)   --
    else
        self.p:pen(COLOR_WHITE)
        self.p:key_pen(COLOR_LIGHTRED)
        self:render_rows()
    end
    self:render_menu()
end

function HistScreen:onInput(keys)
    if keys.LEAVESCREEN then
        self:dismiss()
    end
    if not self.rows:has_rows() then
        return
    end
    if keys.SECONDSCROLL_DOWN or keys.SECONDSCROLL_UP or keys.SECONDSCROLL_PAGEDOWN or keys.SECONDSCROLL_PAGEUP then
        self.rows:update_scroll(
            ((keys.SECONDSCROLL_DOWN or keys.SECONDSCROLL_PAGEDOWN) and 1 or -1),  -- scroll delta
            ((keys.SECONDSCROLL_PAGEUP or keys.SECONDSCROLL_PAGEDOWN) and true or false), -- scroll by page
            {wrap = true}
        )
    elseif keys.CUSTOM_U or keys.CUSTOM_R then
        self.rows:stage_row(keys.CUSTOM_U and 0 or 1)
    elseif keys.CUSTOM_SHIFT_U or keys.CUSTOM_SHIFT_R then
        self.rows:stage_marked(keys.CUSTOM_SHIFT_U and 0 or 1)
    elseif keys.CUSTOM_SHIFT_D then
        self.rows:delete_history()
    elseif keys.CUSTOM_D then
        if self.rows:has_marked_rows() then
            self.rows:delete_marked_rows()
        else
            self.rows:delete_row()
        end
    elseif keys.CUSTOM_Z then
        self.rows:zoom_row()
    elseif keys.SELECT then
        self.rows:mark_row()
    elseif self:propagateMoveKeys(keys) then
        return
    end
end

--
-- Local functions
--

function history()
    HistScreen():show()
end

return _ENV
