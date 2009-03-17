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
#include <verlihub/cserverdc.h>
#include <verlihub/script_api.h>
#include "callbacks.h"
#include "cluainterpreter.h"
#include "cpilua.h"
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

namespace nScripts
{

cLuaInterpreter::cLuaInterpreter(string scriptname) : mScriptName(scriptname)
{
	mL = lua_open();
}

cLuaInterpreter::~cLuaInterpreter()
{
	char * args[] = { NULL };
	if(mL) {
		CallFunction("UnLoad", args);
		lua_close(mL);
	}
	clean();
}

bool cLuaInterpreter::Init()
{

#ifdef HAVE_LUA_5_1
	luaL_openlibs(mL);
#else
	luaopen_base(mL);
	luaopen_table(mL);
	luaopen_io(mL);
	luaopen_string(mL);
	luaopen_math(mL);
	luaopen_debug(mL);
	luaopen_loadlib(mL);
#endif

	lua_newtable(mL);

	RegisterFunction("SendDataToUser",    &_SendToUser); /* back compatibility */
	RegisterFunction("SendToUser",    &_SendToUser);
	RegisterFunction("SendDataToAll",     &_SendToClass); /* back compatibility */
	RegisterFunction("SendToClass",     &_SendToClass);
	RegisterFunction("SendToAll",     &_SendToAll);
	RegisterFunction("SendPMToAll",       &_SendPMToAll);
	RegisterFunction("CloseConnection",   &_Disconnect); /* back compatibility */
	RegisterFunction("Disconnect",   &_Disconnect);
	RegisterFunction("DisconnectByName",   &_Disconnect); /* back compatibility */
	RegisterFunction("GetUserCC",   &_GetUserCC);
	RegisterFunction("GetMyINFO",         &_GetMyINFO);
	RegisterFunction("GetUpTime",         &_GetUpTime);
	RegisterFunction("RegBot",          &_RegBot);
	RegisterFunction("AddRobot",          &_RegBot); /* back compatibility */
	RegisterFunction("UnRegBot",          &_UnRegBot);
	RegisterFunction("DelRobot",          &_UnRegBot); /* back compatibility */
	RegisterFunction("GetHubIp",          &_GetHubIp);
	RegisterFunction("GetHubSecAlias",          &_GetHubSecAlias);
	RegisterFunction("AddRegUser",          &_AddRegUser);
	RegisterFunction("DelRegUser",          &_DelRegUser);
	
	RegisterFunction("GetUserClass",      &_GetUserClass);
	RegisterFunction("GetUserHost",       &_GetUserHost);
	RegisterFunction("GetUserIP",         &_GetUserIP);
	RegisterFunction("Ban",               &_Ban);
	RegisterFunction("KickUser",          &_KickUser);
	RegisterFunction("ParseCommand",      &_ParseCommand);
	RegisterFunction("SetConfig",         &_SetConfig);
	RegisterFunction("GetConfig",         &_GetConfig);
	
	
	RegisterFunction("SQLQuery",          &_SQLQuery);
	RegisterFunction("SQLFetch",          &_SQLFetch);
	RegisterFunction("SQLFree",           &_SQLFree);
	
	RegisterFunction("GetUsersCount",     &_GetUsersCount);
	RegisterFunction("GetTotalShareSize", &_GetTotalShareSize);
	RegisterFunction("GetNickList",       &_GetNickList);
	RegisterFunction("GetBots",       &_GetBots);
	RegisterFunction("GetTempRights",       &_GetTempRights);
	RegisterFunction("SetTempRights",       &_SetTempRights);
	
	
	
	lua_setglobal(mL, "VH");

#ifdef HAVE_LUA_5_1
	int status = luaL_dofile(mL, (char *)mScriptName.c_str());
#else
	int status = lua_dofile(mL, (char *)mScriptName.c_str());
#endif

	if(status) {
		unsigned char *error = (unsigned char *) luaL_checkstring (mL, 1);
		ReportLuaError((char *) error);
		return false;
	}
	
	lua_pushstring(mL,VERSION); lua_setglobal(mL,"_PLUGINVERSION");
	return true;
}

void cLuaInterpreter::Load()
{
	//Call Main first if exists
	char * args[] = { NULL };
	CallFunction("Main", args);
}

void cLuaInterpreter::ReportLuaError(char * error)
{
	if(cpiLua::me && cpiLua::me->log_level) {
		string error2 = "[ Lua ERROR ] ";
		error2.append(error);
		cServerDC * server = cServerDC::sCurrentServer;
		if(server) SendPMToAll( (char *) error2.c_str(), (char *) server->mC.hub_security.c_str(), 3, 10);
	}
	char * args[] = { error, NULL };
	// Dropped because of crash
	//CallFunction("OnError", args);
}

void cLuaInterpreter::RegisterFunction(const char *fncname, int (*fncptr)(lua_State *))
{
	lua_pushstring(mL, fncname);
	lua_pushcfunction(mL, fncptr);
	lua_rawset(mL, -3);
}

bool cLuaInterpreter::CallFunction(char * func, char * args[])
{
	lua_settop(mL, 0);
	int base = lua_gettop(mL);
	lua_pushliteral(mL, "_TRACEBACK");
	lua_rawget(mL, LUA_GLOBALSINDEX);
	lua_insert(mL, base);
	
	lua_getglobal(mL, func);
	
	if(lua_isnil(mL, -1))
	{
		// function not exists
		lua_pop(mL, -1); // remove nil value
		lua_remove(mL, base); // remove _TRACEBACK
	}
	else
	{
		int i=0;
		while(args[i] != NULL)
		{
			lua_pushstring(mL, args[i]);
			i++;
		}
		
		int result = lua_pcall(mL, i, 1, base);
		if(result)
		{
			const char *msg = lua_tostring(mL, -1);
			if(msg == NULL)
				msg = "(unknown LUA error)";
			cout << "LUA error: " << msg << endl;
			ReportLuaError( (char *) msg);
			lua_pop(mL, 1);
			//lua_remove(mL, base); // remove _TRACEBACK
			return true;
		}
		
		int val = (int)lua_tonumber(mL, -1);
		lua_pop(mL, 1);
		
		lua_remove(mL, base); // remove _TRACEBACK
		
		if(!(bool)val)
			return false;
	}
	
	return true;
}

};
