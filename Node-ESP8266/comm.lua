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
	-- {["topic/0"]=0,["topic/1"]=1,topic2=2}
end
function M.enable_config()
	M.m:subscribe("/config/"..M.name.."/file/+", 2, function(conn) print("#config_enabled") end)
end
function M.close( )
	M.m:close();
end
return M
