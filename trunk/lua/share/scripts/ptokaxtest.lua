require("ptokax") -- don't forget to include this line at the begining of the file if you want to deal with ptokax scripts like this...

botname = "LUABOT"

function DataArrival(curUser, sData)
	if curUser.bOperator then
		curUser:SendData("<"..botname.."> Hello operator|")
	else
		curUser:SendData("<"..botname.."> Hello user|")
	end
	
	SendPmToAll(botname, "Begin test...")
	
	SendPmToAll(botname, "Total number of users: "..frmHub:GetUsersCount())
	
	hubname = frmHub:GetHubName()
	SendPmToAll(botname, "Hub name is: "..hubname)
	
	frmHub:SetHubName("test")
	if(frmHub:GetHubName() == "test") then
		SendPmToAll(botname, "Testing writing/reading hub name [passed]")
	else
		SendPmToAll(botname, "Testing writing/reading hub name [failed]")
	end
	frmHub:SetHubName(hubname)
	
	SendPmToAll(botname, "Hub description is: "..frmHub:GetHubDescr())
	
	SendPmToAll(botname, "Hub redirection address is: "..frmHub:GetRedirectAddress())
	
	SendPmToAll(botname, "Hub registration server(s): "..frmHub:GetRegServer())
	
	SendPmToAll(botname, "Number of allowed users: "..frmHub:GetMaxUsers())
	
	SendPmToAll(botname, "Setting minimum share size to 3GB")
	frmHub:SetMinShare(3, 3)
	
	minshare = frmHub:GetMinShare()
	--frmHub:SetMinShare(0, 0)
	SendPmToAll(botname, "is it 3GB? "..minshare.." bytes")
	
	SendPmToAll(botname, "Total share amount: "..frmHub:GetCurrentShareAmount())
	
	SendPmToAll(frmHub:GetOpChatName(), "I'm the OpChat!")
	
	SendPmToAll(frmHub:GetHubBotName(), "I'm the Hub bot!")
	
	SendPmToAll(botname, "This message came with data arrival: "..sData)
	
	res, uclass = VH:GetUserClass(botname)
	SendPmToAll(botname, "Class of "..botname..": "..uclass)
	
	return 1
end

function NewUserConnected(curUser)
	curUser:SendData("<"..botname.."> NewUserConnected called!")
	return 1
end

function OpConnected(curUser)
	curUser:SendData("<"..botname.."> OpConnected called!")
	return 1
end

function UserDisconnected(curUser)
	curUser:SendData("<"..botname.."> UserDisconnected called!")
	return 1
end

function OpDisconnected(curUser)
	curUser:SendData("<"..botname.."> OpDisconnected called!")
	return 1
end

function OnTimer()
	--print("OnTimer called!")
	return 1
end

frmHub:RegBot(botname)
frmHub:RegBot(botname.."_test")
frmHub:UnregBot(botname.."_test")
