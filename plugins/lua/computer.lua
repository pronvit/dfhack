local _ENV = mkmodule('plugins.computer')

utils = require 'utils'

Computer = defclass(Computer)
function Computer:init()
    self.id = __new_computer()
    if self.id < 0 then
        error('computer creation failed')
    end
end

function Computer:register_display(stockpile)
    if not __register_display(self.id, stockpile) then
        error('failed to register display')
    end
end

function Computer:displays()
    return __list_displays(self.id)
end

function Computer:__gc()
    print('__gc')
end

return _ENV
