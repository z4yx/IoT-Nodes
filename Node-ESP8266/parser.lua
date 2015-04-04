local parser = {}

function parser.parse(name,topic,data)
	print(topic)
	if topic:find("^/control/")then
		local tok = topic:match("^/control/.+/(.+)")
		if tok then
			print("#control+"..tok.."+"..data)
		end
	end
end

return parser