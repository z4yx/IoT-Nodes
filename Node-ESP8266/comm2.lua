local M = {}

function M.init( name )
    M.m = mqtt.Client()
    M.parser = require 'parser'
    M.name = name
    M.m:lwt("/events/"..name.."/offline", "", 2, 0)
    M.m:on("connect", function(con)
        print ("#connected")
        M.m:subscribe("/control/"..M.name.."/+",2)
    end)
    M.m:on("offline", function(con) print ("#offline") end)

    M.m:on("message", function(conn, topic, data) 
        M.parser.parse(M.name, topic, data)
    end)
end
return M
