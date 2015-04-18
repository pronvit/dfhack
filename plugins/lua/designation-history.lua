local _ENV = mkmodule('plugins.designation-history')

function history()
    local hist = get_history()
    for i, entry in pairs(hist) do
        print(i)
        printall(entry)
    end
end

return _ENV
