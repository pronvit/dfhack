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
    print 'history'
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
    self.block = dfhack.maps.getTileBlock(args.x, args.y, args.z)
    self.bx = args.x % 16
    self.by = args.y % 16
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
    find_priority_event(self.block)[self.bx][self.by] = self.priority
end

Designation = defclass(Designation)
function Designation:init()
    self.stages = {}
end

function Designation:set(stage, coords)
    if stage ~= 1 and stage ~= 2 then error('Invalid stage: ' .. stage) end
    local x1 = coords[1]
    local x2 = coords[4]
    local y1 = coords[2]
    local y2 = coords[5]
    local z1 = coords[3]
    local z2 = coords[6]
    self.minx = math.min(x1, x2)
    self.maxx = math.max(x1, x2)
    self.miny = math.min(y1, y2)
    self.maxy = math.max(y1, y2)
    self.minz = math.min(z1, z2)
    self.maxz = math.max(z1, z2)
    local s = {}
    self.stages[stage] = s
    for x = self.minx, self.maxx do
        s[x] = {}
        for y = self.miny, self.maxy do
            s[x][y] = {}
            for z = self.minz, self.maxz do
                s[x][y][z] = Tile{x=x, y=y, z=z}
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
    for x = self.minx, self.maxx do
        for y = self.miny, self.maxy do
            for z = self.minz, self.maxz do
                self.stages[stage][x][y][z]:write()
            end
        end
    end
end

return _ENV
