local _ENV = mkmodule('plugins.designation-history')

gui = require 'gui'
guidm = require 'gui.dwarfmode'

function scroll_index(index, delta, min, max, opts)
    if not opts then opts = {} end
    if opts.wrap == nil then opts.wrap = true end
    index = index + delta
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
    return index
end

function page_number(cursor, page_height)
    local page
    if cursor <= page_height then
        page = 1
    else
        page = math.floor((cursor -1) / page_height) + 1
    end
    return page
end

HistScreen = defclass(HistScreen, guidm.MenuOverlay)

function HistScreen:init(opts)
    self.super.init(self)
    -- reverse - list newest first
    self.history = {}
    for i, v in pairs(opts.history) do
        self.history[#opts.history - i + 1] = v
    end
    local _, cols = dfhack.screen.getWindowSize()
    self.start = 1
    self.page = 1
    self.cursor = 1
    self.page_height = math.max(cols - 10, 20)
    self.start_max = #self.history - self.page_height + 1
end

function HistScreen:page_start()
    return self.page * self.page_height - self.page_height + 1
end

function HistScreen:page_end()
    return math.min(self.page * self.page_height, #self.history)
end

function HistScreen:render(_p)
    self.super.render(self, _p)
    local p = gui.Painter.new_wh(
        self.df_layout.menu.x1 + 1,
        self.df_layout.menu.y1 + 1,
        self.df_layout.menu.width - 2,
        self.df_layout.menu.height - 2
    )
    if #self.history == 0 then
        p:string('History empty', {fg = COLOR_LIGHTRED})
        return
    end
    for i = self:page_start(), self:page_end() do
        local hitem = self.history[i]
        p:string(('%2i: (%2ix%2ix%i) %s'):format(i, hitem.dimx, hitem.dimy, hitem.dimz, hitem.desc),
            {fg = i == self.cursor and COLOR_WHITE or COLOR_GREY})
        p:newline()
    end
    p:string('  Page ' .. self.page .. ' of ' .. math.floor(#self.history / self.page_height + 1), {fg = COLOR_GREEN})
    p:seek(0, p.y2 - 3)
    p:key('CUSTOM_U'):string(': Undo, ')
    p:key('CUSTOM_R'):string(': Redo, ')
    p:key('CUSTOM_Z'):string(': Zoom')
    p:newline()
    p:key('SECONDSCROLL_DOWN'):key('SECONDSCROLL_UP'):key('SECONDSCROLL_PAGEDOWN'):key('SECONDSCROLL_PAGEUP')
    p:string(': Scroll')
end

function HistScreen:onInput(keys)
    if keys.LEAVESCREEN then
        self:dismiss()
    end
    if #self.history == 0 then
        return
    end
    local hitem = self.history[self.cursor]
    if keys.SECONDSCROLL_DOWN or keys.SECONDSCROLL_UP or keys.SECONDSCROLL_PAGEDOWN or keys.SECONDSCROLL_PAGEUP then
        self.cursor = scroll_index(
            self.cursor,
            ((keys.SECONDSCROLL_DOWN or keys.SECONDSCROLL_PAGEDOWN) and 1 or -1) *
                ((keys.SECONDSCROLL_PAGEUP or keys.SECONDSCROLL_PAGEDOWN) and self.page_height or 1),
            1,
            #self.history,
            {wrap = true}
        )
        self.page = page_number(self.cursor, self.page_height)
    elseif keys.CUSTOM_U or keys.CUSTOM_R then
        reset_stage(hitem.id, keys.CUSTOM_U and 0 or 1)
    elseif keys.CUSTOM_Z then
        local pos = {x = hitem.centerx, y = hitem.centery, z = hitem.centerz}
        df.global.cursor:assign(pos)
        dfhack.gui.revealInDwarfmodeMap(pos, true)
    end
end

function history()
    HistScreen({history=get_history()}):show()
end

return _ENV
