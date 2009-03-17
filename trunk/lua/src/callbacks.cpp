/***************************************************************************
 *   Copyright (C) 2004 by Janos Horvath                                   *
 *   bourne@freemail.hu                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define ERR_PARAM "wrong parameter(s)"
#define ERR_CALL "call error"
 
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "cpilua.h"
#include "callbacks.h"
#include <verlihub/cserverdc.h>
#include <verlihub/cuser.h>
#include <verlihub/script_api.h>
#include <iostream>
#include <string>

using namespace std;
using namespace nDirectConnect;

cServerDC * GetCurrentVerlihub()
{
	return (cServerDC *)cServerDC::sCurrentServer;
}

int _SendToUser(lua_State *L)
{
	string data, nick;

	if(lua_gettop(L) == 3) {
		if(!lua_isstring(L, 2))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		data = (char *)lua_tostring(L, 2);
		if(!lua_isstring(L, 3))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 3);
		if(!SendDataToUser((char *)data.c_str(), (char *)nick.c_str()))
		{
			luaerror(L, ERR_CALL);
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:SendToUser; expected 2 arguments but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _SendToClass(lua_State *L)
{
	string data;
	int min_class, max_class;

	if(lua_gettop(L) == 4) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		data = (char *)lua_tostring(L, 2);
		if(!lua_isnumber(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		min_class = (int)lua_tonumber(L, 3);
		if(!lua_isnumber(L, 4)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		max_class = (int)lua_tonumber(L, 4);
		if(!SendToClass((char*)data.c_str(), min_class, max_class)) {
			luaerror(L, ERR_CALL);
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:SendToClass; expected 3 arguments but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _SendToAll(lua_State *L)
{
	string data;

	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		data = (char *)lua_tostring(L, 2);
		if(!SendToAll((char*)data.c_str())) {
			luaerror(L, ERR_CALL);
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:SendToAll; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _SendPMToAll(lua_State *L)
{
	string data, from;
	int min_class = 0, max_class = 10;

	if(lua_gettop(L) > 4) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		data = (char *) lua_tostring(L, 2);
		if(!lua_isstring(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		from = (char *) lua_tostring(L, 3);
		
		if(lua_isnumber(L, 4)) {
			min_class = (int) lua_tonumber(L, 4);
		}
		
		if(lua_isnumber(L, 5)) {
			max_class = (int) lua_tonumber(L, 5);
		}
		
		if(!SendPMToAll((char*)data.c_str(), (char *)from.c_str(), min_class, max_class)) {
			luaerror(L, ERR_CALL);
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:SendPMToAll; expected at least 3 arguments but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _Disconnect(lua_State *L)
{
	string nick;

	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		if(!CloseConnection((char*)nick.c_str())) {
			luaerror(L, ERR_CALL);
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:Disconnect; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _GetMyINFO(lua_State *L)
{
	string nick;
	char *myinfo;
	int result = 1;

	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		myinfo = GetMyINFO( (char*) nick.c_str());
		if(strlen(myinfo) > 0) result = 1;
		else {
			result = 0;
			myinfo = "User not found";
		}
		lua_pushboolean(L, result);
		lua_pushstring(L, myinfo);
		return 2;
	} else {
		luaL_error(L, "Error calling VH:GetMyINFO; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _GetUserCC(lua_State *L)
{
	char *nick, *cc;
	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 2);
		cc = GetUserCC(nick);	
		if(cc == NULL) {
			lua_pushboolean(L, 0);
			luaerror(L, "User not found");
		} else {
			lua_pushboolean(L, 1);
			lua_pushstring(L, cc);
		}
		return 2;
	}
	else {
		luaL_error(L, "Error calling VH:GetUserCC; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _GetNickList(lua_State *L)
{
	char *nicklist;
	int result = 1;
	if(lua_gettop(L) == 1) {
#if HAVE_GETNICKLIST
		nicklist = GetNickList();
#else
		nicklist = "not available";
#endif
		if(strlen(nicklist) < 1 || !strcmp(nicklist, "not available")) result = 0;
		else result = 1;
		lua_pushboolean(L, result);
		lua_pushstring(L, nicklist);
		return 2;
	} else {
		luaL_error(L, "Error calling VH:GetNickList; expected  0 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _GetUserClass(lua_State *L)
{
	string nick;
	int uclass;

	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 2);
		uclass = GetUserClass((char*)nick.c_str());
		lua_pushboolean(L, 1);
		lua_pushnumber(L, uclass);
		return 2;
	} else {
		luaL_error(L, "Error calling VH:GetNickList; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _GetUserHost(lua_State *L)
{
	string nick, host;

	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 2);
		host = GetUserHost((char*)nick.c_str());
		lua_pushboolean(L, 1);
		lua_pushstring(L, (char *)host.c_str());
		return 2;
	} else {
		luaL_error(L, "Error calling VH:GetUserHost; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _GetUserIP(lua_State *L)
{
	string nick, ip;
	
	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 2);
		ip = GetUserIP((char*)nick.c_str());
		lua_pushboolean(L, 1);
		lua_pushstring(L, (char *)ip.c_str());
		return 2;
	} else {
		luaL_error(L, "Error calling VH:GetUserIP; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _Ban(lua_State *L)
{
	string nick, op, reason;
	unsigned howlong;
	int bantype;

	if(lua_gettop(L) == 6)
	{
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		if(!lua_isstring(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		op = (char *) lua_tostring(L, 3);
		if(!lua_isstring(L, 4)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		reason = (char *) lua_tostring(L, 4);
		if(!lua_isnumber(L, 5)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		howlong = (unsigned) lua_tonumber(L, 5);
		if(!lua_isnumber(L, 6)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		bantype = (int) lua_tonumber(L, 6);
		if(!Ban((char *) nick.c_str(), op, reason, howlong, bantype)) {
			luaerror(L, "User not found");
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:Ban; expected 5 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _KickUser(lua_State *L)
{
	string nick, op, data;

	if(lua_gettop(L) == 4)
	{
		if(!lua_isstring(L, 2))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		op = (char *)lua_tostring(L, 2);
		if(!lua_isstring(L, 3))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 3);
		if(!lua_isstring(L, 4))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		data = (char *)lua_tostring(L, 4);
		if(!KickUser((char *)op.c_str(), (char *)nick.c_str(), (char *)data.c_str()))
		{
			luaerror(L, ERR_CALL);
			return 2;
		}
	}
	else
	{
		luaL_error(L, "Error calling VH:KickUser; expected 3 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _ParseCommand(lua_State *L)
{
	// NOTE: this one is not implemented yet!
	string data;

	if(lua_gettop(L) == 2)
	{
		if(!lua_isstring(L, 2))
		{
			luaerror(L, ERR_PARAM);
			return 2;
		}
		data = (char *)lua_tostring(L,2);
		if(!ParseCommand((char *)data.c_str()))
		{
			luaerror(L, ERR_CALL);
			return 2;
		}
	}
	else
	{
		luaerror(L, ERR_PARAM);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _SetConfig(lua_State *L)
{
	string config_name, var, val;

	if(lua_gettop(L) == 4) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		config_name = (char *)lua_tostring(L, 2);
		if(!lua_isstring(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		var = (char *)lua_tostring(L, 3);
		if(!lua_isstring(L, 4)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		val = (char *)lua_tostring(L, 4);
		if(!SetConfig((char *)config_name.c_str(), (char *)var.c_str(), (char *)val.c_str())) {
			luaerror(L, ERR_CALL);
			return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:SetConfig; expected 3 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _GetConfig(lua_State *L)
{
	char *val = new char[64];
	string config_name, var;
	int size;

	if(lua_gettop(L) == 3) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		config_name = (char *)lua_tostring(L, 2);
		if(!lua_isstring(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		var = (char *)lua_tostring(L, 3);
		size = GetConfig((char *)config_name.c_str(), (char *)var.c_str(), val, 64);
		if(size < 0) {
			luaerror(L, "Error calling GetConfig API");
			return 2;
		}
		if(size >= 63) {
			delete [] val;
			val = new char[size+1];
			GetConfig((char *)config_name.c_str(), (char *)var.c_str(), val, size+1);
		}
		lua_pushboolean(L, 1);
		lua_pushstring(L, val);
		delete [] val;
		val = 0;
		return 2;
	} else {
		luaL_error(L, "Error calling VH:GetConfig; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _RegBot(lua_State *L)
{
	string nick, desc, speed, email, share;
	int uclass;

	if(lua_gettop(L) == 7) {
		cServerDC *server = GetCurrentVerlihub();
		if(server == NULL) {
			luaerror(L, "Error getting server");
			return 2;
		}

		cpiLua *pi = (cpiLua *)server->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);
		if(pi == NULL) {
			luaerror(L, "Error getting LUA plugin");
			return 2;
		}
		
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *)lua_tostring(L, 2);
		
		if(!lua_isnumber(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		uclass = (int)lua_tonumber(L, 3);
		
		if(!lua_isstring(L, 4)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		desc = (char *)lua_tostring(L, 4);

		if(!lua_isstring(L, 5)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		speed = (char *)lua_tostring(L, 5);

		if(!lua_isstring(L, 6)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		email = (char *)lua_tostring(L, 6);
		
		if(!lua_isstring(L, 7)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		share = (char *)lua_tostring(L, 7);
		
		cPluginRobot *robot = pi->NewRobot(nick, uclass);
		
		if(robot != NULL) {
			server->mP.Create_MyINFO(robot->mMyINFO, robot->mNick, desc, speed, email, share);
			robot->mMyINFO_basic = robot->mMyINFO;
			
			cLuaInterpreter *li = FindLua(L);
			if(li == NULL) {
				luaerror(L,"Lua not found");
				return 2;
			}
			//li->addBot(nick, share, robot->mMyINFO, uclass);
			li->addBot((char *) nick.c_str(), (char *) share.c_str(), (char *) robot->mMyINFO.c_str(), uclass);
			string omsg = "$Hello ";
			omsg+= robot->mNick;
			server->mHelloUsers.SendToAll(omsg, server->mC.delayed_myinfo, true);
			omsg = server->mP.GetMyInfo(robot, eUC_NORMUSER);
			server->mUserList.SendToAll(omsg, true, true);
			if(uclass >= 3)
				server->mUserList.SendToAll(server->mOpList.GetNickList(), true);
		} else {
		    luaerror(L, "Error adding bot; it may already exist");
		    return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:RegBot; expected 6 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _UnRegBot(lua_State *L)
{
	string nick;
	
	if(lua_gettop(L) == 2) {
		cServerDC *server = GetCurrentVerlihub();
		if(server == NULL) {
			luaerror(L, "Error getting server");
			return 2;
		}
		
		cpiLua *pi = (cpiLua *)server->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);
		if(pi == NULL) {
			luaerror(L, "Error getting LUA plugin");
			return 2;
		}
	
		if(!lua_isstring(L, 2)) {
		    luaerror(L, ERR_PARAM);
		    return 2;
		}
		nick = (char *)lua_tostring(L, 2);
		cPluginRobot *robot = (cPluginRobot *)server->mUserList.GetUserByNick(nick);
		if(robot != NULL) {
			cLuaInterpreter *li = FindLua(L);
			if(li == NULL) {
				luaerror(L,"Lua not found");
				return 2;
			}
			li->delBot((char *) nick.c_str() );
			pi->DelRobot(robot);
		} else {
		    luaerror(L, "Bot doesn't exist");
		    return 2;
		}
	} else {
		luaL_error(L, "Error calling VH:UnRegBot; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _SQLQuery(lua_State *L)
{
	if(lua_gettop(L) == 2) {
		cServerDC *server = GetCurrentVerlihub();
		if(server == NULL) {
			luaerror(L, "Error getting server");
			return 2;
		}
		
		cpiLua *pi = (cpiLua *)server->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);
		if(pi == NULL) {
			luaerror(L, "Error getting LUA plugin");
			return 2;
		}		
		if(!lua_isstring(L, 2)) {
		    luaerror(L, ERR_PARAM);
		    return 2;
		}
		pi->mQuery->Clear();
		pi->mQuery->OStream() << (char *) lua_tostring(L, 2);
		pi->mQuery->Query();
		int i = pi->mQuery->StoreResult();
		
		lua_pushboolean(L, 1);
		if(i > 0)
			lua_pushnumber(L, i);
		else
			lua_pushnumber(L, 0);
		return 2;
	} else {
		luaL_error(L, "Error calling VH:UnRegBot; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _SQLFetch(lua_State *L)
{
	if(lua_gettop(L) == 2) {
		cServerDC *server = GetCurrentVerlihub();
		if(server == NULL) {
			luaerror(L, "Error getting server");
			return 2;
		}
		
		cpiLua *pi = (cpiLua *)server->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);
		if(pi == NULL) {
			luaerror(L, "Error getting LUA plugin");
			return 2;
		}
		
		if(!lua_isnumber(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		
		int r = (int)lua_tonumber(L, 2);
		
		if(!pi->mQuery->GetResult()) {
			luaerror(L, "No result");
			return 2;
		}
		
		pi->mQuery->DataSeek(r);
		
		MYSQL_ROW row;
		
		if(!(row = pi->mQuery->Row()))
		{
			luaerror(L, "Error fetching row");
			return 2;
		}
		
		lua_pushboolean(L, 1);
		
		int j = 0;
		while(j < pi->mQuery->Cols()) {
			lua_pushstring(L, (char *)row[j]);
			j++;
		}
		return j + 1;
	} else {
		luaL_error(L, "Error calling VH:SQLFetch; expected 1 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _SQLFree(lua_State *L)
{
	if(lua_gettop(L) == 1) {	
		cServerDC *server = GetCurrentVerlihub();
		if(server == NULL) {
			luaerror(L, "Error getting server");
			return 2;
		}
		
		cpiLua *pi = (cpiLua *)server->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);
		if(pi == NULL)
		{
			luaerror(L, "Error getting LUA plugin");
			return 2;
		}
		
		pi->mQuery->Clear();	
	} else {
		luaL_error(L, "Error calling VH:SQLFree; expected 0 argument but got %d", lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	lua_pushboolean(L, 1);
	return 1;
}

int _GetVHCfgDir(lua_State *L)
{
	char * path;
	if(lua_gettop(L) == 1) {
		lua_pushboolean(L, 1);
		lua_pushstring(L,  GetVHCfgDir());
		return 2;	
	} else {
		luaL_error(L, "Error calling VH:GetVHCfgDir; expected 0 argument but got %d", lua_gettop(L) -1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}

int _GetUpTime(lua_State *L)
{
	cServerDC *server = GetCurrentVerlihub();
	if(server == NULL) {
		luaerror(L, "Error getting server");
		return 2;
	}
	cTime upTime;
	upTime = server->mTime;
	upTime -= server->mStartTime;
	lua_pushboolean(L, 1);
	lua_pushnumber(L, upTime.Sec());
	return 2;
}

int _GetHubIp(lua_State *L)
{
	cServerDC *server = GetCurrentVerlihub();
	if(server == NULL) {
		luaerror(L, "Error getting server");
		return 2;
	}
	lua_pushboolean(L, 1);
	lua_pushstring(L, (char *) server->mAddr.c_str());
	return 2;
}

int _GetHubSecAlias(lua_State *L)
{
	cServerDC *server = GetCurrentVerlihub();
	if(server == NULL) {
		luaerror(L, "Error getting server");
		return 2;
	}
	lua_pushboolean(L, 1);
	lua_pushstring(L, (char *) server->mC.hub_security.c_str());
	return 2;
}

int _GetUsersCount(lua_State *L)
{
	lua_pushboolean(L, 1);
	lua_pushnumber(L, GetUsersCount());
	return 2;
}

int _GetTotalShareSize(lua_State *L)
{
	lua_pushboolean(L, 1);
	lua_pushnumber(L, GetTotalShareSize());
	return 2;
}
/*
int _ReportUser(lua_State *L)
{
	string nick, message;
	
	if(lua_gettop(L) == 3) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		if(!lua_isstring(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		message = (char *) lua_tostring(L, 3);
		
		cUser *usr = GetUser(nick);
		if (!usr) {
			luaerror(L, "User not found");
			return 2;
		} else if(!usr->mxConn) {	
			luaerror(L, "User found but it's a bot");
			return 2;
		}
		else {
			cServerDC *server = GetCurrentVerlihub();
			if (!server) {
				luaerror(L, "Error getting server");
				return 2;
			}
			server->ReportUserToOpchat(usr->mxConn, message, false);
			lua_pushboolean(L, 1);
			return 1;
		}	
	} else {
		luaL_error(L, "Error calling VH:ReportUser; expected 1 argument but got %d", lua_gettop(L) -1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
}
*/
int _SetTempRights(lua_State *L)
{
	string nick, temp;
	int rights[3];
	
	if(lua_gettop(L) == 3) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		if(!lua_istable(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		char * keys[] = { "ctm", "pm", "mc", NULL };
		int i = 0;
		while (keys[i] != NULL) {
			cout << "Key is " << keys[i] << endl;
			lua_pushstring(L, (char*) keys[i]);
			cout << "Pushed key" << endl;
			lua_gettable(L, -2);
			cout << "Getting value out of the stack" << endl;
			cout << " and value from key ctm is " << lua_tonumber(L, -1) << endl;
			if(keys[i] == "mc") {
			//	rights[2] = (int) lua_tonumber(L, -1);
			} else if(keys[i] == "pm") {
			//	rights[1] = (int) lua_tonumber(L, -1);
			}
			else if(keys[i] == "ctm") {
			//	rights[0] = (int) lua_tonumber(L, -1);
			} else {
				luaL_error(L, "Error calling VH:SetTempRights; provided an invalid table");
				lua_pushboolean(L, 0);
				lua_pushnil(L);
			}
			i++;
		}
		/*
		for(int i = 0; i < 3; i++) {
			cout << "Value is " << (int) lua_tonumber(L, -1) << endl;
			if(!lua_isnumber(L, -1)) {
				luaL_error(L, "Error calling VH:SetTempRights; value must be an integer");
				lua_pushboolean(L, 0);
				lua_pushnil(L);
			}
			temp = (char *) lua_tostring(L, -2);
			cout << "Key found is " << temp << " and it's value is " << (int) lua_tonumber(L, -1) << endl;;
			if(temp == "mc") {
				rights[2] = (int) lua_tonumber(L, -1);
			} else if(temp == "pm") {
				rights[1] = (int) lua_tonumber(L, -1);
			}
			else if(temp == "ctm") {
				rights[0] = (int) lua_tonumber(L, -1);
			} else {
				luaL_error(L, "Error calling VH:SetTempRights; provided an invalid table");
				lua_pushboolean(L, 0);
				lua_pushnil(L);
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);		*/
	} else {
		luaL_error(L, "Error calling VH:SetTempRights; expected 3 argument but got %d", lua_gettop(L) -1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}
	
}

int _GetTempRights(lua_State *L)
{
	string nick;
	int rights[3];
	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		if(GetTempRights((char *) nick.c_str(), rights)) {
			lua_settop(L,0);
			lua_pushboolean(L, 1);
			lua_newtable(L);
			
			int i = lua_gettop(L);
			lua_pushliteral(L, "mc");
			lua_pushnumber(L, rights[2]);
			lua_rawset(L, i);
		
			lua_pushliteral(L, "pm");
			lua_pushnumber(L, rights[1]);
			lua_rawset(L, i);
			
			lua_pushliteral(L, "ctm");
			lua_pushnumber(L, rights[0]);
			lua_rawset(L, i);
			return 2;
		} else {
			luaerror(L, "User not found");
			return 2;	
		}
		
	} else {
		luaL_error(L, "Error calling VH:GetTempRights; expected 1 argument but got %d", lua_gettop(L) -1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}	
}

cLuaInterpreter *FindLua(lua_State *L)
{
	cLuaInterpreter *temp;
	int vSize = cpiLua::me->Size();
	for(int i = 0; i < vSize; i++) {
		temp = cpiLua::me->mLua[i];
		if(temp->mL == L) return temp;
	}
	return NULL;
}

int _GetBots(lua_State *L)
{
	
	cLuaInterpreter *li;
	int vSize,z,k,key=0;
	vSize = cpiLua::me->Size();
	lua_newtable(L);
	z = lua_gettop(L);
	
	for(int i = 0; i < vSize; i++) {
		li = cpiLua::me->mLua[i];
		
		for(int j = 0; j < li->botList.size(); j++) {
			lua_pushnumber(L, ++key);
			
				lua_newtable(L);
				int k = lua_gettop(L);
				lua_pushliteral(L, "sScriptname");
				lua_pushstring(L, (char *) li->mScriptName.c_str());
				lua_rawset(L, k);
				
				lua_pushliteral(L, "sNick");
				lua_pushstring (L, li->botList[j]->uNick);
				lua_rawset(L, k);
			
				lua_pushliteral(L, "iClass");
				lua_pushnumber(L, li->botList[j]->uClass);
				lua_rawset(L, k);
			
				lua_pushliteral(L, "iShare");
				lua_pushstring(L, li->botList[j]->uShare);
				lua_rawset(L, k);
										
				lua_pushliteral(L, "sMyINFO");
				lua_pushstring(L, li->botList[j]->uMyINFO);
				lua_rawset(L, k);
				
			lua_rawset(L, z);
		}
	}
	return 1;
}

int _AddRegUser(lua_State *L)
{
	string nick, password, op;
	int uClass, args = lua_gettop(L);
	if(args >= 4 && args <= 5) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		if(!lua_isstring(L, 3)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		password = (char *) lua_tostring(L, 2);
		if(!lua_isnumber(L, 4)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		uClass = (int) lua_tonumber(L, 4);
		if(!lua_isnumber(L, 5)) op = "";
		else op = (char *) lua_tostring(L, 5);
		AddRegUser( (char *) nick.c_str(), uClass, (char *) password.c_str(), (char *) op.c_str());
		lua_pushboolean(L, 1);
		return 1;
	}
	else {
		luaL_error(L, "Error calling VH:AddRegUser; expected at least 3 arguments and max 4 but got %d", lua_gettop(L) -1);
		lua_pushboolean(L, 0);
		return 1;
	}	
}

int _DelRegUser(lua_State *L)
{
	string nick;
	if(lua_gettop(L) == 2) {
		if(!lua_isstring(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}
		nick = (char *) lua_tostring(L, 2);
		DelRegUser( ( char *) nick.c_str());
	}
	else {
		luaL_error(L, "Error calling VH:DelRegUser; expected 1 argument but got %d", lua_gettop(L) -1);
		lua_pushboolean(L, 0);
		return 1;
	}	
	
}

void luaerror(lua_State *L, const char * errstr)
{
	lua_pushboolean(L, 0);
	lua_pushstring(L, errstr);
}
