-- Warning displayed on startup when using a pre-release version of DFHack

local gui = require 'gui'
local dlg = require 'gui.dialogs'

local message = {
    {pen=COLOR_LIGHTRED,
        text='This is a pre-release version of DFHack (' .. dfhack.VERSION .. ')'}, NEWLINE,
    'Crashes and broken features are to be expected. Report problems at:', NEWLINE,
    '- https://github.com/dfhack/dfhack (official DFHack repository)', NEWLINE,
    '- http://tinyurl.com/dfhack-ng-plus (Bay12 forum thread)', NEWLINE,
    '- #dfhack on irc.freenode.net',
}

print('')

for k,v in ipairs(message) do
    if type(v) == 'table' then
        dfhack.color(v.pen)
        dfhack.print(v.text)
    else
        dfhack.color(COLOR_YELLOW)
        dfhack.print(v)
    end
end

dfhack.color(COLOR_RESET)
print('\n')

dlg.showMessage('Pre-release warning', message, COLOR_YELLOW)
