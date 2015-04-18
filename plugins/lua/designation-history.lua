local _ENV = mkmodule('plugins.designation-history')

dfhack.storage = dfhack.storage or {}
dfhack.storage.designation_history = dfhack.storage.designation_history or {}
dfhack.storage.designation_history.history = dfhack.storage.designation_history.history or {}
dfhack.storage.designation_history.index = dfhack.storage.designation_history.index or 0
local d_history = dfhack.storage.designation_history.history
local d_history_idx = dfhack.storage.designation_history.index

function undo()
    if d_history[d_history_idx] then
        d_history[d_history_idx]:undo()
        d_history_idx = d_history_idx - 1
    end
end

function history()
    print(('history: %d items'):format(#d_history))
    printall(d_history)
end

function before_select(...)
    assert(not current_designation)
    select(false, ...)
end

function after_select(...)
    assert(current_designation)
    select(true, ...)
end

function select(complete, ...)
    args = {...}
    if not complete then
        current_designation = Designation()
        current_designation:set(1, {...})
    else
        current_designation:set(2, {...})
        table.insert(d_history, current_designation)
        d_history_idx = #d_history
        current_designation = nil
    end
end

local function find_priority_event(block)
    for i, evt in pairs(block.block_events) do
        if df.block_square_event_designation_priorityst:is_instance(evt) then
            return evt.priority
        end
    end
end

Tile = defclass(Tile)
function Tile:init(args)
    self.block = dfhack.maps.getTileBlock(args[1], args[2], args[3])
    self.bx = args[1] % 16
    self.by = args[2] % 16
    local des = self.block.designation[self.bx][self.by]
    self.dig = des.dig
    self.smooth = des.smooth
    self.traffic = des.traffic
    local priority_event = find_priority_event(self.block)
    if priority_event then
        self.priority = priority_event[self.bx][self.by]
    else
        self.priority = 0
    end
    return self
end

function Tile:write()
    local des = self.block.designation[self.bx][self.by]
    des.dig = self.dig
    des.smooth = self.smooth
    des.traffic = self.traffic
    local evt = find_priority_event(self.block)
    if evt then
        evt[self.bx][self.by] = self.priority
    end
end

Designation = defclass(Designation)
function Designation:init()
    self.stages = {}
end

function Designation:set(stage, coords)
    if stage ~= 1 and stage ~= 2 then error('Invalid stage: ' .. stage) end
    self.x1 = math.min(coords[1], coords[4])
    self.x2 = math.max(coords[1], coords[4])
    self.y1 = math.min(coords[2], coords[5])
    self.y2 = math.max(coords[2], coords[5])
    self.z1 = math.min(coords[3], coords[6])
    self.z2 = math.max(coords[3], coords[6])
    self.dimx = self.x2 - self.x1 + 1
    self.dimy = self.y2 - self.y1 + 1
    self.dimz = self.z2 - self.z1 + 1
    local s = {}
    self.stages[stage] = s
    local dimy = self.dimy
    local dimz = self.dimz
    for x = self.x1, self.x2 do
        for y = self.y1, self.y2 do
            for z = self.z1, self.z2 do
                s[x * dimy * dimz + y * dimz + z] = Tile{x, y, z}
            end
        end
    end
end

function Designation:undo()
    self:reset_stage(1)
end

function Designation:redo()
    self:reset_stage(2)
end

function Designation:reset_stage(stage)
    local s = self.stages[stage]
    local dimy = self.dimy
    local dimz = self.dimz
    for x = self.x1, self.x2 do
        for y = self.y1, self.y2 do
            for z = self.z1, self.z2 do
                s[x * dimy * dimz + y * dimz + z]:write()
            end
        end
    end
end

return _ENV
