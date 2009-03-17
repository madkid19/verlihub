-- PtokaX LUA script interface for Verlihub by bourne.
-- Note: There is many incompatibily differences between PtokaX and Verlihub. I try to make all possible functions, but some of them mayl not be implemented.
-- This script makes a PtokaX like environment for PtokaX LUA scripts, thus you can run them (if you are lucky)
-- Anyway don't forget that PtokaX uses LUA 4.x but Verlihub goes with LUA 5.0, thus you have to change some LUA functions in order to use PtokaX scripts with Verlihub.
-- There is an important thing you should do with all your PtokaX scripts. At the begining of the script place a require("ptokax") but don't forget to set the LUA_PATH environment
-- variable to whatever directory where ptokax.lua located (this script)

--- [ Create frmHub object ] ---

frmHub = {}
TimerTicks = 0
TimerTemp = 0
TimerRun = false

function frmHub:RegBot(botname)
	return VH:AddRobot(botname, 10, "description", "speed\7", "mail", "0")
end

function frmHub:UnregBot(botname)
	return VH:DelRobot(botname)
end

function frmHub:GetUsersCount()
	res, totalusers =  VH:GetUsersCount()
	if res then
		return totalusers
	end
	return 0
end

function frmHub:GetHubName()
	res, val = VH:GetConfig("config", "hub_name")
	if res then
		return val
	end
	return 0
end

function frmHub:SetHubName(string)
	res, err = VH:SetConfig("config" , "hub_name", string)
	if res then
		return 1
	end
	return 0
end

function frmHub:GetHubDescr()
	res, val = VH:GetConfig("config", "hub_desc")
	if res then
		return val
	end
	return 0
end

function frmHub:SetHubDescr(string)
	res, err = VH:SetConfig("config" , "hub_desc", string)
	if res then
		return 1
	end
	return 0
end

function frmHub:GetRedirectAddress()
	res, val = VH:GetConfig("config", "redir_host0")
	if res then
		return val
	end
	return 0
end

function frmHub:SetRedirectAddress(string)
	res, err = VH:SetConfig("config" , "redir_host0", string)
	if res then
		return 1
	end
	return 0
end

-- todo: don't know is it useful
function frmHub:GetRedirectAll()
	-- returns 0 or 1
	return 0
end

-- todo: don't know is it useful
function frmHub:SetRedirectAll(number)
	-- number = 0 or 1
	return 1
end

-- todo: if you specify redir_hostx host then Verlihub redirects when full
function frmHub:GetRedirectFull()
	-- returns 0 or 1
	return 1
end

-- todo: if you specify redir_hostx host then Verlihub redirects when full
function frmHub:SetRedirectFull(number)
	-- number = 0 or 1
	return 1
end

function frmHub:GetRegServer()
	res, val = VH:GetConfig("config", "hublist_host")
	if res then
		return val
	end
	return 0
end

function frmHub:SetRegServer(string) 
	res, err = VH:SetConfig("config" , "hublist_host", string)
	if res then
		return 1
	end
	return 0
end

-- todo: find out what is this, maybe registers all user who joins to hub
function frmHub:SetAutoRegister(number)
	-- number = 0 or 1
	return 1
end

function frmHub:GetMaxUsers()
	res, val = VH:GetConfig("config", "max_users")
	if res then
		return val
	end
	return 0
end

function frmHub:SetMaxUsers(number)
	res, err = VH:SetConfig("config" , "max_users", number)
	if res then
		return 1
	end
	return 0
end

function frmHub:GetMinShare()
	res, val = VH:GetConfig("config", "min_share")
	if res then
		return val
	end
	return 0
end

function frmHub:SetMinShare(amount, unit)
	-- unit = number (unit: 0=byte, 1=KByte,2=MByte, 3=GByte)
	
	if unit < 0 or unit > 3 then unit = 0 end
	
	if unit == 1 then unit = 1024 
	elseif unit == 2 then unit = 1024 * 1024
	elseif unit == 3 then unit = 1024 * 1024 * 1024 
	else unit = 1
	end

	ms = amount * unit
	
	res, err = VH:SetConfig("config" , "min_share", ms)
	if res then
		return 1
	end
	return 0
end

function frmHub:GetCurrentShareAmount()
	res, totalshare = VH:GetTotalShareSize()
	if res then
		return totalshare
	end
	return 0
end

function frmHub:GetOpChatName()
	res, val = VH:GetConfig("config", "opchat_name")
	if res then
		return val
	end
	return 0
end

function frmHub:GetHubBotName()
	res, val = VH:GetConfig("config", "hub_security")
	if res then
		return val
	end
	return 0
end

-- why is it useful?
function frmHub:EnableSearchData(n) 
	-- n = 1 or 0 (1-enable,0-disable)
	-- enables/disables passing of $Search, $MultiSearch and $SR to current script
	return 1
end

-- why is it useful?
function frmHub:EnableFullData(n)
	-- n = 1 or 0 (1-enable,0-disable)
	-- enables/disables passing of ALL incomming data to current script
	-- NOTE: may have significant influence on hub's performance. Use it wisely!
	return 1
end

-- [ Helper functions for real event handlers ] --

function VH_OnUserLogin(nick)
	curUser = CreateUserObj(nick)
	if curUser.bOperator then
		if OpConnected ~= nil then return OpConnected(curUser) end
	else
		if NewUserConnected ~= nil then return NewUserConnected(curUser) end
	end
end

function VH_OnUserLogout(nick)
	curUser = CreateUserObj(nick)
	if curUser.bOperator then
		if OpDisconnected ~= nil then return OpDisconnected(curUser) end
	else
		if UserDisconnected ~= nil then return UserDisconnected(curUser) end
	end
end

function VH_OnParsedMsgAny(nick,data)
	if DataArrival ~= nil then DataArrival(CreateUserObj(nick), data) return 1 end
end

function VH_OnTimer()
	if TimerRun then
		if TimerTicks > 0 then
			TimerTicks = TimerTicks - 1
		else
			OnTimer()
			TimerTicks = TimerTemp
		end
	end
end

function CreateUserObj(nick)
	curUser = {}
	
	curUser.sName = nick
	
	res, class = VH:GetUserClass(nick)
	if not res then
		curUser.bOperator = nil
	else
		if class >= 4 then
			curUser.bOperator = true
		end
	end
	
	res, ip = VH:GetUserIP(nick)
	if not res then
		curUser.sIP = "unknown"
	else
		curUser.sIP = ip
	end
	
	curUser.iVersion = "todo!" -- this can be extraced from MyINFO
	
	res, myinfo = VH:GetMyINFO(nick)
	if not res then
		curUser.sMyInfoString = "unknown"
	else
		curUser.sMyInfoString = myinfo
	end
	
	curUser.iProfile = "todo!" -- This is very PtokaX thing to hold user rights
	
	function curUser:SendData(data)
		return VH:SendDataToUser(data, curUser.sName)
	end
	
	function curUser:SendPM(from, data)
		return VH:SendDataToUser("$To: "..curUser.sName.." From: "..from.." $<"..from.."> "..data.."|", curUser.sName)
	end
	
	function curUser:Disconnect()
		return VH:CloseConnection(curUser.sName)
	end
	
	function curUser:Ban()
		--because of VH:Ban() not implemented on API level, we will use VH:KickUser() with _BAN_xy in reason
		return VH:KickUser(frmHub:GetOpChatName(), curUser.sName, "_BAN_1h")
	end
	
	function curUser:NickBan()
		-- First kick user, and ban permanently.
		VH:KickUser(frmHub.GetOpChatName(), curUser.sName, "_BAN_");

		-- Then update banlist to say nick-ban.
		sql = "UPDATE banlist SET ban_type=2, ip='_nickban_' WHERE nick='"..curUser.sName.."'";
		res, err = VH:SQLQuery(sql);

		-- Check if successful.
		if (not res) then
			-- Didn't succeed, return with error.
			return 0, err;
		end;
	end
	
	function curUser:TempBan()
		return VH:KickUser(frmHub:GetOpChatName(), curUser.sName, "TempBan")
	end
	
	function curUser:TimeBan(period)
		return VH:KickUser(frmHub:GetOpChatName(), curUser.sName, "_BAN_"..period.."s")
	end
	
	return curUser
end

--- [ Global functions ] ---

function SendToNick(nick, data)
	return VH:SendDataToUser(data, nick)
end

function SendToAll(from, data) -- from field is optional! todo, check the num of args, and if its 1 then only data provided
	return VH:SendDataToAll(data.."|", 1, 10)
end

function SendPmToAll(from, data)
	return VH:SendPMToAll(data, from, 1, 10)
end

function SendPmToNick(to, from, data)
	return VH:SendDataToUser("$To: "..to.." From: "..from.." $<"..from.."> "..data.."|", to)
end

function SendToOps(from, data)
	return VH:SendDataToAll("<"..from.."> "..data.."|", 3, 10)
end

function SendPmToOps(from, data)
	return VH:SendPMToAll(data, from, 3, 10)
end

function GetItemByName(nick)
	return CreateUserObj(nick)
end

function DisconnectByName(nick)
	res, err = VH:CloseConnection(nick)
	if res then
		return 1
	end
	return 0
end

function AddRegUser(nick, pass, level)
	sql = "INSERT INTO reglist (nick, class, pwd_change) VALUES ("..nick..", "..level..", "..pass..")";
	res, err = VH:SQLQuery (sql);

	if (not res) then
		VH:SendDataToAll ("Error! Could not register user "..nick.." with class: "..class..".|", 5, 10);
		VH:SendDataToAll ("Error: "..err.."|", 5, 10);
		return 0;
	end;

	return 1;
end

function DelRegUser(nick)
	sql = "DELETE FROM reglist WHERE nick = "..nick;
	res, err = VH:SQLQuery (sql);

	if (not res) then
		VH:SendDataToAll ("Error! Could not delete regged user "..nick..".|", 5, 10);
		VH:SendDataToAll ("Error: "..err.."|", 5, 10);
		return 0;
	end;

	return 1;
end

function SetTimer(time)
	TimerTicks = time / 1000
	TimerTemp = TimerTicks
end

function StartTimer()
	TimerRun = true
end

function StopTimer()
	TimerRun = false
end

function Unban(what)
	-- Declare local variables, for later use.
	local type = "";
	local _, _, a, b, c, d = string.find (what, "(%d+).(%d+).(%d+).(%d+)");
	local name = string.find (what, "(%s+)");

	-- Check what type of content has been sent.
	if (tonumber (a) and tonumber (b) and tonumber (c) and tonumber (d)) then
		-- Content is an IP-address, setting type & correct content.
		what = tonumber (a).."."..tonumber (b).."."..tonumber (c).."."..tonumber (d);
		type = "ip";
	else if (name ~= "") then
		-- Content is a nick, setting type & correct content.
		what = name;
		type = "nick";
	else
		-- Unknown content, send warning and return false.
		VH:SendDataToAll ("Warning! Unknown parametre sent, doing nothing");
		return 0;
	end;

	-- Delete the date from the database.
	sql = "DELETE FROM banlist WHERE "..type.."='"..what.."'";
	res, err = VH:SQLQuery (sql);

	-- Check if successfult
	if (not res) then
		-- Was not, send error and return false.
		VH:SendDataToAll ("Error! Could not delete "..type.."-ban, SQL: "..sql.."|", 5, 10);
		VH:SendDataToAll ("Error: "..err.."|", 5, 10);
		return 0;
	end;

	return 1;
end

function ClearTempBan()
	sql = "DELETE FROM banlist WHERE date_limit != NULL";
	res, err = VH:SQLQuery (sql);

	if (not res) then
		VH:SendDataToAll ("Error! Could not clear temp-bans, SQL: "..sql.."|", 5, 10);
		VH:SendDataToAll ("Error: "..err.."|", 5, 10);
		return 0;
	end;

	return 1;
end
