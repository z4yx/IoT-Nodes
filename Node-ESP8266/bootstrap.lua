print('bootstrap script working...');
function writeFile(filename,content)
file.open(filename,"w")
file.write(content)
file.close()
end
local file_list = {}
m=mqtt.Client('bootstrap'..node.chipid())
function next_file()
  if #file_list == 0 then
    m:close();
    print('\035initialized')
    return
  end
  m:subscribe("/bootstrap/file/"..table.remove(file_list,#file_list), 2)
end
m:on("message",function(conn,topic,data)
  print(topic)
  if topic=="/bootstrap/list" then
    for i in string.gmatch(data, "[^\n\r]+") do
      table.insert(file_list,i)
    end
    next_file()
    return
  end
  local fn = topic:match("/bootstrap/file/(.+)")
  if fn and data~=nil then
    writeFile(fn, data)
  end
  next_file()
end)
-- _,_,gw=wifi.sta.getip()
gw='192.168.1.2'
m:connect(gw, 1883, 0, function(conn) 
  m:subscribe("/bootstrap/list", 2)
end)
