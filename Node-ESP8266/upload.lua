function writeFile( filename, content )
    file.open(filename, "w")
    file.write(content)
    file.close()
end
m = mqtt.Client()
m:on("message", function(conn, topic, data) 
      print(topic..":")
      if topic:find("/config/")==1 then
        local fn = topic:match("/%a+/.+/file/(.+)")
        if fn then
            writeFile(fn, data)
            return
        end
      end

      if data ~= nil then
        print(data)
      end
end)

m:connect("192.168.1.30", 1883, 0, function(conn) 
    print("connected") 
    m:subscribe("/config/name/file/+", 2, function(conn) print("subscribe success") end)
end)

