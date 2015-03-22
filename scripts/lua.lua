-- Execute lua commands interactively or from files.

local HELP = [[Lua interpreter
lua:
    Interactive lua interpreter
lua STATEMENT
:lua STATEMENT
    Execute a Lua statement. ":lua" is recommended if the statement contains spaces.
lua -f|--file FILENAME
    Run the script indicated by FILENAME
lua -s|--save FILENAME
    Run the script indicated by FILENAME in the current save folder
lua -r|--reset|--reset-env
    Reset the environment used by the interactive interpreter
lua -h|-?|--help
    Display this help
]]

local args={...}
local cmd = args[1]
prompt_env = prompt_env or {}

if cmd=="--file" or cmd=="-f" then
    local f,err=loadfile (args[2])
    if f==nil then
        qerror(err)
    end
    dfhack.safecall(f,table.unpack(args,3))
elseif cmd=="--save" or cmd=="-s" then
    if df.global.world.cur_savegame.save_dir=="" then
        qerror("Savefile not loaded")
    end
    local fname=args[2] or "dfhack.lua"
    fname=string.format("data/save/%s/%s",df.global.world.cur_savegame.save_dir,fname)
    local f,err=loadfile (fname)
    if f==nil then
        qerror(err)
    end
    dfhack.safecall(f,table.unpack(args,3))
elseif cmd=="--reset-env" or cmd=="--reset" or cmd=="-r" then
    prompt_env = {}
elseif cmd=="--help" or cmd:match('-?[h?]$') then
    print(HELP)
elseif cmd:sub(1,1) == "-" then
    dfhack.printerr('Unrecognized command: ' .. cmd .. '\nRun "lua -h" for help.')
elseif cmd~=nil then
    cmd = table.concat(args, ' ')
    -- Support some of the prefixes allowed by dfhack.interpreter
    local prefix
    if string.match(cmd, "^[~@!]") then
        prefix = string.sub(cmd, 1, 1)
        cmd = 'return '..string.sub(cmd, 2)
    end

    local f,err=load(cmd,'=(lua command)', 't')
    if f==nil then
        qerror(err)
    end

    local rv = table.pack(dfhack.safecall(f,table.unpack(args,2)))

    if rv[1] and prefix then
        print(table.unpack(rv,2,rv.n))
        if prefix == '~' then
            printall(rv[2])
        elseif prefix == '@' then
            printall_ipairs(rv[2])
        end
    end
else
    dfhack.interpreter("lua", "lua.history", nil, prompt_env)
end
