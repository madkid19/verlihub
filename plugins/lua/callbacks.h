/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz)                *
*                    Janos Horvath (bourne at freemail dot hu) 2004-05    *
*                                                                         *
*   Copyright (C) 2006-2011 by Verlihub Project                           *
*   devs at verlihub-project dot org                                      *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#ifndef CALLBACKS_H
#define CALLBACKS_H

extern "C"
{
    #include <lua.h>
}
#include "cpilua.h"
#include "cluainterpreter.h"
namespace nVerliHub {
	int _SendToUser(lua_State *);
	int _SendToClass(lua_State *);
	int _SendToAll(lua_State *);
	int _SendPMToAll(lua_State *);
	int _Disconnect(lua_State *);
	int _StopHub(lua_State *);
	int _GetMyINFO(lua_State *);
	int _GetUserCC(lua_State *);
	int _GetIPCC(lua_State *);
	int _GetVHCfgDir(lua_State *);
	int _GetUpTime(lua_State *L);
	int _RegBot(lua_State *);
	int _UnRegBot(lua_State *);
	int _EditBot(lua_State *L);
	int _IsBot(lua_State *L);
	int _GetHubIp(lua_State *);
	int _GetHubSecAlias(lua_State *);
	int _SetConfig(lua_State *);
	int _GetConfig(lua_State *);
	int _ReportUser(lua_State *);
	int _GetUserClass(lua_State *);
	int _GetUserHost(lua_State *);
	int _GetUserIP(lua_State *);
	int _IsUserOnline(lua_State *L);
	int _InUserSupports(lua_State *L);
	int _GetNickList(lua_State *);
	int _GetOPList(lua_State *);
	int _GetBotList(lua_State *);
	int _GetBots(lua_State *);
	int _Ban(lua_State *);
	int _KickUser(lua_State *);
	int _ParseCommand(lua_State *);

	int _SQLQuery(lua_State *);
	int _SQLFetch(lua_State *);
	int _SQLFree(lua_State *);

	int _GetUsersCount(lua_State *L);
	int _GetTotalShareSize(lua_State *);

	int _GetTempRights(lua_State *);
	int _SetTempRights(lua_State *);
	int _AddRegUser(lua_State *);
	int _DelRegUser(lua_State *);
	int _GetTopic(lua_State *L);
	int _SetTopic(lua_State *L);
	nLuaPlugin::cLuaInterpreter *FindLua(lua_State *lua_state);
	void luaerror(lua_State *, const char *);
}; // namespace nVerliHub

#endif
