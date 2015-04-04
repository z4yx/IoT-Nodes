local M = {}

function M.connect( server, port )
	M.m:connect(server, port, 0) -- 1 for secure
end
function M.publish(cate, key, value, qos, retain)
	M.m:publish("/"..cate.."/"..M.name.."/"..key,value,qos,retain)
end
function M.subscribe( topic, qos )
	M.m:subscribe(topic,qos, function(conn) print("#subscribed") end)
end
function M.multi_subscribe(topic)
	M.m:subscribe(topic, function(conn) print("#multi-subscribed") end)
end
function M.close( )
	M.m:close();
end

local comm2 = require('comm2')
setmetatable(M, {__index = comm2})

return M