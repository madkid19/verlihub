----------[ Global variables ]----------

botname = "LUABOT"

----------[ Callback functions ]----------
-- Note: You can reach all api functions throught VH object for example: 'VH:SendDataToAll(data, nick)' where data and nick
-- both strings. 

-- Every callback functions will have at lease 1 return value, watch this: 'result, data = VH:GetUserClass(nick)'
-- If VH:GetUserClass fails for some reason, then 'result' will be 'false', and the 'data' will contain the 'error message',
-- when success 'result' will be true and the 'data' will contain the 'userclass'. If you call a function that doesn't have
-- to return any data, then on failure 'data' will contain the 'error message' but on success 'data' will take a nil value.

-- Here is the list of all API functions:

-- result, data = VH:SendDataToUser(data, nick)
-- result, data = VH:SendDataToAll(data, min_class, max_class)
-- result, data = VH:CloseConnection(nick)
-- result, data = VH:GetMyINFO(nick)
-- result, data = VH:GetUserClass(nick)
-- result, data = VH:GetUserHost(nick)
-- result, data = VH:GetUserIP(nick)
-- result, data = VH:Ban(nick, howlong, bantime)
-- result, data = VH:KickUser(nick, reason)
-- result, data = VH:ParseCommand(command)
-- result, data = VH:SetConfig(config_name, varible, value)
-- result, data = VH:GetConfig(config_name, varible)
-- result, data = VH:AddRobot(nick, description, speed, email, share)
-- result, data = VH:DelRobot(nick)

-- result, data = VH:SQLQuery(query)
-- result, data = VH:SQLFetch(row)
-- result, data = VH:SQLFree()

----------[ Event handler functions ]----------

--VH_OnNewConn
function VH_OnNewConn(ip)
	print("In script.lua's VH_OnNewConn function")
	print("IP:" .. ip)
	return 1
end

--VH_OnCloseConn
function VH_OnCloseConn(ip)
	print("In script.lua's VH_OnCloseConn function")
	print("IP:" .. ip)
	return 1
end

--VH_OnParsedMsgChat
function VH_OnParsedMsgChat(nick, data)
	print("In script.lua's VH_OnParsedMsgChat function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	
	data = data .. " "
	
	_, _, command, robot = string.find(data, "^(%S+)%s+(%S+)")
	
	if(command == "#addrobot") then
		res, err = VH:AddRobot(robot, "test bot", "Hub\7", "email@domain.tld", "0")
		if res then
			VH:SendDataToAll("$Hello "..robot.."|",1, 10)
		else
			su("Robot error: "..err, nick)
		end
		return 1
	end
	
	if(command == "#delrobot")
	then
		res, err = VH:DelRobot(robot)
		if not res then
			su("Robot error: "..err, nick)
		end
		return 1
	end
	
	res, myinfo = VH:GetMyINFO(nick)
	res, uclass = VH:GetUserClass(nick)
	if res then
		su("Hi, your class is:"..uclass.." and your myinfo is:"..myinfo, nick)
	end
	
	res, err = VH:SetConfig("config" , "hub_name", data)
	if not res then
		su("Oops, SetConfig error: "..err, nick)
	else
		su("Setting hubname to: "..data, nick)
	end
	
	res, val = VH:GetConfig("config", "hub_name")
	if res then
		su("The hub name is: "..val, nick)
	else
		su("Oops, GetConfig error: "..err, nick)
	end
	
	return 1
end

--VH_OnParsedMsgPM
function VH_OnParsedMsgPM(from, data, to)
	print("In script.lua's VH_OnParsedMsgPM function")
	print("From:" .. from)
	print("To:" .. to)
	print("Data:" .. data)
	return 1
end

--VH_OnParsedMsgSupport
function OnParsedMsgSupport(nick, data)
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnParsedMsgMyPass
function OnParsedMsgMyPass(nick, data)
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnParsedMsgSearch
function VH_OnParsedMsgSearch(nick,data)
	print("In script.lua's VH_OnParsedMsgSearch function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnParsedMsgRevConnectToMe
function VH_OnParsedMsgConnectToMe(nick,othernick)
	print("In script.lua's VH_OnParsedMsgRevConnectToMe function")
	print("Nick:" .. nick)
	print("Other:" .. othernick)
	return 1
end

--VH_OnParsedMsgConnectToMe
function VH_OnParsedMsgConnectToMe(nick,othernick, ip, port)
	print("In script.lua's VH_OnParsedMsgConnectToMe function")
	print("Nick:" .. nick)
	print("Other:" .. othernick)
	print("Ip:port:" .. ip .. ":" .. port)
	return 1
end

--VH_OnParsedMsgSR
function VH_OnParsedMsgSR(nick, data)
	print("In script.lua's VH_OnParsedMsgSR function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnParsedMsgMyINFO
function VH_OnParsedMsgMyINFO(nick, data)
	print("In script.lua's VH_OnParsedMsgMyINFO function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnParsedMsgValidateNick
function VH_OnParsedMsgValidateNick(nick)
	print("In script.lua's VH_OnParsedMsgValidateNick function")
	print("Nick:" .. nick)
	return 1
end

--VH_OnParsedMsgAny
-- Note: This one is crazy, it will be called on any message that go across on verlihub.
function VH_OnParsedMsgAny(ip, data)
	--print("In script.lua's VH_OnParsedMsgAny function")
	--print("IP:" .. ip)
	--print("Data:" .. data)
	return 1
end

--VH_OnUnknownMsg
function VH_OnUnknownMsg(nick, data)
	print("In script.lua's VH_OnUnknownMsg function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnOperatorCommand
function VH_OnOperatorCommand(nick, data)
	print("In script.lua's VH_OnOperatorCommand function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnOperatorKicks
function VH_OnOperatorKicks(op, nick, data)
	print("In script.lua's VH_OnOperatorKicks function")
	print("OP:" .. op)
	print("Nick:" .. nick)    
	print("Data:" .. data)
	return 1
end

--VH_OnOperatorDrops
function VH_OnOperatorDrops(op, nick)
	print("In script.lua's VH_OnOperatorDrops function")
	print("OP:" .. op)
	print("Nick:" .. nick)    
	return 1
end

--VH_OnUserCommand
function VH_OnUserCommand(nick, data)
	print("In script.lua's VH_OnUserCommand function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnValidateTag
function VH_OnValidateTag(nick, data)
	print("In script.lua's VH_OnValidateTag function")
	print("Nick:" .. nick)
	print("Data:" .. data)
	return 1
end

--VH_OnUserLogin
function VH_OnUserLogin(nick)
	--print("In script.lua's VH_OnUserLogin function")
	--print("Nick:" .. nick)
	
	so("Nick: "..nick)
	
	res, host = VH:GetUserHost(nick)
	if not res then
		so("Error:"..host)
		return 1
	else
		so("Host: "..host)
	end
	
	res, ip = VH:GetUserIP(nick)
	if not res then
		so("Error:"..ip)
		return 1
	else
		so("IP: "..ip)
	end
	
	query = "SELECT count(*) FROM test2 WHERE nick='"..nick.."'"
	res, n = VH:SQLQuery(query)
	if not res then
		so("Error in query: "..query)
		so("Error: "..n)
		return 1
	end
	
	if n > 0 then
		query = "UPDATE test2 SET host='"..host.."', ip='"..ip.."' WHERE nick='"..nick.."'"
	else
		query = "INSERT INTO test2 VALUES ('"..nick.."','"..host.."','"..ip.."')"
	end

	res, err = VH:SQLQuery(query)
	if not res then
		so("Error in query: "..query)
		so("Error: "..err)
	end
	
	return 1
end

--VH_OnUserLogout
function VH_OnUserLogout(nick)
	print("In script.lua's VH_OnUserLogout function")
	print("Nick:" .. nick)
	return 1
end

--VH_OnTimer
-- Note: This is called very often, useful for timing :)
function VH_OnTimer()
	--print("In script.lua's VH_OnTimer function")
	return 1
end

--VH_OnNewReg
function VH_OnNewReg(op, nick, class)
	print("In script.lua's VH_OnNewReg function")
	print("OP:" .. op)
	print("Nick:" .. nick)
	print("Class:" .. class)
	return 1
end

--VH_OnNewBan
function VH_OnNewBan(op, ip, nick, reason)
	print("In script.lua's VH_OnNewBan function")
	print("OP:" .. op)
	print("IP:" .. ip)
	print("Nick:" .. nick)
	print("Reason:" .. reason)
	return 1
end

function so(data)
  VH:SendDataToAll("<"..botname.."> "..data.."|",4, 10)
end

function sa(data)
  VH:SendDataToAll("<"..botname.."> "..data.."|",1, 10)
end

function su(data, nick)
  VH:SendDataToUser("<"..botname.."> "..data.."|", nick)
end

----------[ Global stuff ]----------
-- This part runs when script loaded

query = "CREATE TABLE IF NOT EXISTS test2 (nick varchar(30) PRIMARY KEY, host varchar(30), ip varchar(15))"
res, err = VH:SQLQuery(query)
if not res then
	so("Error in query: "..query)
	so("Error: "..err)
end

query = "SELECT nick, host, ip  FROM test2"
res, data = VH:SQLQuery(query)
if not res then
	so(data)
else
	for x=0,data-1 do
		res, nick, host, ip = VH:SQLFetch(x)
		if not res then
			so("Error: "..nick) -- in this case it holds the error message
			break
		else
			sa("User: "..nick.." from "..host.." was here with IP: "..ip)
		end
	end
end

res, err = VH:AddRobot(botname, 10, "test bot", "Hub\7", "email@domain.tld", "0")
if not res then
	so("Error: "..err)
else
	VH:SendDataToAll("$Hello "..botname.."|",1, 10)
	so("Robot "..botname.." created and waiting for your orders.")
end
