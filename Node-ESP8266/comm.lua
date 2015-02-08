local M = {}

local function writeFile( filename, content )
	file.open(filename, "w")
	file.write(content)
	file.close()
end

function M.init( name )
	M.m = mqtt.Client()
	M.name = name
	M.m:lwt("/events/"..name.."/offline", "", 1, 0)
	M.m:on("connect", function(con) print ("#connected") end)
	M.m:on("offline", function(con) print ("#offline") end)

	M.m:on("message", function(conn, topic, data) 
		if topic:find("/config/")then
			local fn = topic:match("/%a+/.+/file/(.+)")
			if fn then
				writeFile(fn, data)
				return
			end
		end
		if data ~= nil then
			print('#'..topic.."+"..data)
		end
	end)
end
function M.connect( server, port )
	-- for secure: m:connect("192.168.11.118", 1880, 1)
	M.m:connect(server, port, 0)
end
function M.publish_value( key, value )
	M.m:publish("/values/"..M.name.."/"..key,value,1,1)
end
function M.publish_event( key, value )
	M.m:publish("/events/"..M.name.."/"..key,value,2,0)
end
function M.subscribe( topic, qos )
	M.m:subscribe(topic,qos, function(conn) print("#subscribe success") end)
end
function M.multi_subscribe(topic)
	M.m:subscribe(topic, function(conn) print("#multi_subscribe success") end)
	-- {["topic/0"]=0,["topic/1"]=1,topic2=2}
end
function M.enable_config( )
	M.m:subscribe("/config/"..M.name.."/file/+", 2, function(conn) print("#enable_config success") end)
end
function M.close( )
	M.m:close();
end
return M
