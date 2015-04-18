local _ENV = mkmodule('plugins.designation-history')

function history()
    local hist = get_history()
    printall(hist)
end

return _ENV
