local parser = {}

local function writeFile( filename, content )
	file.open(filename, "w")
	file.write(content)
	file.close()
end

function parser.parse(name,topic,data)
	print(topic)
	if topic:find("^/config/")then
		local fn = topic:match("^/config/.+/file/(.+)")
		if fn then
			print("Updating "..fn)
			writeFile(fn, data)
			return
		end
	elseif topic:find("^/control/")then
		local tok = topic:match("^/control/.+/(.+)")
		if tok then
			print("#control+"..tok.."+"..data)
		end
	end
end

return parser