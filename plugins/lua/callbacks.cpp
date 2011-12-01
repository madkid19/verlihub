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

#define ERR_ARGS "Error calling VH:%s, expected at least %d arguments but got %d."
#define ERR_PARAM "Wrong parameters"
#define ERR_CALL "Call error"
#define ERR_SERV "Error getting server"
#define ERR_PLUG "Error getting Lua plugin"
#define ERR_LIN "Error finding Lua"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "cpilua.h"
#include "callbacks.h"
#include "src/cserverdc.h"
#include "src/cconndc.h"
#include "src/cuser.h"
#include "src/script_api.h"
#include <iostream>
#include <string>
#define lua_tablepush(L, key, val) lua_pushstring(L, key); lua_pushnumber(L, val); lua_settable(L, -3);

using namespace std;
namespace nVerliHub {
	using namespace nSocket;
	using namespace nEnums;
	using namespace nLuaPlugin;

cServerDC * GetServ()
{
	return (cServerDC *)cServerDC::sCurrentServer;
}

int _SendToUser(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "SendToUser", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string data = (char*)lua_tostring(L, 2);
	string nick = (char*)lua_tostring(L, 3);

	if (!SendDataToUser((char*)data.c_str(), (char*)nick.c_str())) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _SendToClass(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "SendToClass", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string data = (char*)lua_tostring(L, 2);
	int min = 0, max = 10;

	if (lua_isnumber(L, 3)) {
		min = (int)lua_tonumber(L, 3);
	}

	if (lua_isnumber(L, 4)) {
		max = (int)lua_tonumber(L, 4);
	}

	if (!SendToClass((char*)data.c_str(), min, max)) {
		luaerror(L, ERR_CALL);
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _SendToAll(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "SendToAll", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string data = (char*)lua_tostring(L, 2);

	if (!SendToAll((char*)data.c_str())) {
		luaerror(L, ERR_CALL);
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _SendPMToAll(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "SendPMToAll", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string data = (char*)lua_tostring(L, 2);
	string from = (char*)lua_tostring(L, 3);
	int min = 0, max = 10;

	if (lua_isnumber(L, 4)) {
		min = (int)lua_tonumber(L, 4);
	}

	if (lua_isnumber(L, 5)) {
		max = (int)lua_tonumber(L, 5);
	}

	if (!SendPMToAll((char*)data.c_str(), (char*)from.c_str(), min, max)) {
		luaerror(L, ERR_CALL);
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _SendToOpChat(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "SendToOpChat", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string msg = (char*)lua_tostring(L, 2);
	serv->mOpChat->SendPMToAll(msg, NULL);
	lua_pushboolean(L, 1);
	return 1;
}

int _ReportUser(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "ReportUser", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	string msg = (char*)lua_tostring(L, 3);
	cUser *usr = serv->mUserList.GetUserByNick(nick);

	if ((usr == NULL) || (usr->mxConn == NULL)) {
		luaerror(L, "User not found");
		return 2;
	}

	serv->ReportUserToOpchat(usr->mxConn, msg, false);
	lua_pushboolean(L, 1);
	return 1;
}

int _Disconnect(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "Disconnect", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);

	if (!CloseConnection((char*)nick.c_str())) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _KickUser(lua_State *L)
{
	if (lua_gettop(L) < 4) {
		luaL_error(L, ERR_ARGS, "KickUser", 3, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3) || !lua_isstring(L, 4)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string op = (char*)lua_tostring(L, 2);
	string nick = (char*)lua_tostring(L, 3);
	string rsn = (char*)lua_tostring(L, 4);

	if (!KickUser((char*)op.c_str(), (char*)nick.c_str(), (char*)rsn.c_str())) {
		luaerror(L, "Operator not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _Ban(lua_State *L)
{
	if (lua_gettop(L) < 6) {
		luaL_error(L, ERR_ARGS, "Ban", 5, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3) || !lua_isstring(L, 4) || !lua_isnumber(L, 5) || !lua_isnumber(L, 6)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	string op = (char*)lua_tostring(L, 3);
	string reason = (char*)lua_tostring(L, 4);
	unsigned howlong = (unsigned)lua_tonumber(L, 5);
	int bantype = (int)lua_tonumber(L, 6);

	if (!Ban((char*)nick.c_str(), op, reason, howlong, bantype)) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _GetMyINFO(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "GetMyINFO", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	const char *myinfo = GetMyINFO((char*)nick.c_str());

	if (strlen(myinfo) < 1) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	lua_pushstring(L, myinfo);
	return 1;
}

int _GetUserIP(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "GetUserIP", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	string ip = GetUserIP((char*)nick.c_str());
	lua_pushboolean(L, 1);
	lua_pushstring(L, (char*)ip.c_str());
	return 1;
}

int _GetUserHost(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "GetUserHost", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	string host = GetUserHost((char*)nick.c_str());
	lua_pushboolean(L, 1);
	lua_pushstring(L, (char*)host.c_str());
	return 1;
}

int _GetUserCC(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "GetUserCC", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	char *nick = (char*)lua_tostring(L, 2);
	char *cc = GetUserCC(nick);

	if (cc == NULL) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	lua_pushstring(L, cc);
	return 1;
}

int _GetIPCC(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "GetIPCC", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string ip = (char*)lua_tostring(L, 2);
	string cc = GetIPCC((char*)ip.c_str());
	lua_pushboolean(L, 1);
	lua_pushstring(L, (char*)cc.c_str());
	return 1;
}

int _GetUserClass(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "GetUserClass", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	int cls = GetUserClass((char*)nick.c_str());

	if (cls == -2) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	lua_pushnumber(L, cls);
	return 1;
}

int _GetConfig(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "GetConfig", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string conf = (char*)lua_tostring(L, 2);
	string var = (char*)lua_tostring(L, 3);
	char *val = new char[64];
	int size = GetConfig((char*)conf.c_str(), (char*)var.c_str(), val, 64);

	if (size < 0) {
		luaerror(L, ERR_CALL);
		return 2;
	}

	if (size >= 63) {
		delete [] val;
		val = new char[size + 1];
		GetConfig((char*)conf.c_str(), (char*)var.c_str(), val, size + 1);
	}

	lua_pushboolean(L, 1);
	lua_pushstring(L, val);
	delete [] val;
	val = 0;
	return 1;
}

int _SetConfig(lua_State *L)
{
	if (lua_gettop(L) < 4) {
		luaL_error(L, ERR_ARGS, "SetConfig", 3, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3) || !lua_isstring(L, 4)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string conf = (char*)lua_tostring(L, 2);
	string var = (char*)lua_tostring(L, 3);
	string val = (char*)lua_tostring(L, 4);

	if (!SetConfig((char*)conf.c_str(), (char*)var.c_str(), (char*)val.c_str())) {
		luaerror(L, ERR_CALL);
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _RegBot(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "RegBot", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isnumber(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	cpiLua *pi = (cpiLua*)serv->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);

	if (pi == NULL) {
		luaerror(L, ERR_PLUG);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	int cls = (int)lua_tonumber(L, 3);
	cPluginRobot *robot = pi->NewRobot(nick, cls);

	if (robot == NULL) {
		luaerror(L, "Error adding bot, it may already exist");
		return 2;
	}

	cLuaInterpreter *li = FindLua(L);

	if (li == NULL) {
		luaerror(L, ERR_LIN);
		return 2;
	}

	string desc(""), speed(""), email(""), share("0");

	if (lua_isstring(L, 4)) {
		desc = (char*)lua_tostring(L, 4);
	}

	if (lua_isstring(L, 5)) {
		speed = (char*)lua_tostring(L, 5);
	}

	if (lua_isstring(L, 6)) {
		email = (char*)lua_tostring(L, 6);
	}

	if (lua_isstring(L, 7)) {
		share = (char*)lua_tostring(L, 7);
	}

	serv->mP.Create_MyINFO(robot->mMyINFO, robot->mNick, desc, speed, email, share);
	robot->mMyINFO_basic = robot->mMyINFO;
	li->addBot((char*)nick.c_str(), (char*)share.c_str(), (char*)robot->mMyINFO.c_str(), cls);
	string omsg = "$Hello ";
	omsg += robot->mNick;
	serv->mHelloUsers.SendToAll(omsg, serv->mC.delayed_myinfo, true);
	omsg = serv->mP.GetMyInfo(robot, eUC_NORMUSER);
	serv->mUserList.SendToAll(omsg, true, true);
	if (cls >= 3) serv->mUserList.SendToAll(serv->mOpList.GetNickList(), true);
	lua_pushboolean(L, 1);
	return 1;
}

int _UnRegBot(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "UnRegBot", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	cPluginRobot *robot = (cPluginRobot*)serv->mUserList.GetUserByNick(nick);

	if (robot == NULL) {
		luaerror(L, "Bot not found");
		return 2;
	}

	cpiLua *pi = (cpiLua*)serv->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);

	if (pi == NULL) {
		luaerror(L, ERR_PLUG);
		return 2;
	}

	cLuaInterpreter *li = FindLua(L);

	if (li == NULL) {
		luaerror(L, ERR_LIN);
		return 2;
	}

	li->delBot((char*)nick.c_str());
	pi->DelRobot(robot);
	lua_pushboolean(L, 1);
	return 1;
}

int _EditBot(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "EditBot", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isnumber(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	cpiLua *pi = (cpiLua*)serv->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);

	if (pi == NULL) {
		luaerror(L, ERR_PLUG);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);

	if (!serv->mRobotList.ContainsNick(nick)) {
		luaerror(L, "Bot not found");
		return 2;
	}

	cUserRobot *robot = (cUserRobot*)serv->mRobotList.GetUserBaseByNick(nick);

	if (robot == NULL) {
		luaerror(L, "Bot not found");
		return 2;
	}

	cLuaInterpreter *li = FindLua(L);

	if (li == NULL) {
		luaerror(L, ERR_LIN);
		return 2;
	}

	int cls = (int)lua_tonumber(L, 3);
	string desc(""), speed(""), email(""), share("0");

	if (lua_isstring(L, 4)) {
		desc = (char*)lua_tostring(L, 4);
	}

	if (lua_isstring(L, 5)) {
		speed = (char*)lua_tostring(L, 5);
	}

	if (lua_isstring(L, 6)) {
		email = (char*)lua_tostring(L, 6);
	}

	if (lua_isstring(L, 7)) {
		share = (char*)lua_tostring(L, 7);
	}

	robot->mMyINFO = "";
	serv->mP.Create_MyINFO(robot->mMyINFO, robot->mNick, desc, speed, email, share);
	robot->mMyINFO_basic = robot->mMyINFO;
	li->editBot((char*)nick.c_str(), (char*)share.c_str(), (char*)robot->mMyINFO.c_str(), cls);
	string omsg = serv->mP.GetMyInfo(robot, eUC_NORMUSER);
	serv->mUserList.SendToAll(omsg, false, true);
	if (cls >= 3) serv->mUserList.SendToAll(serv->mOpList.GetNickList(), true);
	lua_pushboolean(L, 1);
	return 1;
}

int _IsBot(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "IsBot", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	cPluginRobot *robot = (cPluginRobot*)serv->mUserList.GetUserByNick(nick);
	lua_pushboolean(L, (robot == NULL) ? 0 : 1);
	return 1;
}

int _GetBots(lua_State *L)
{
	cLuaInterpreter *li;
	int key = 0;
	int s = cpiLua::me->Size();
	int z = lua_gettop(L);
	//lua_pushboolean(L, 1);
	lua_newtable(L);

	for (int i = 0; i < s; i++) {
		li = cpiLua::me->mLua[i];

		for (unsigned int j = 0; j < li->botList.size(); j++) {
			lua_pushnumber(L, ++key);

			lua_newtable(L);
			int k = lua_gettop(L);
			lua_pushliteral(L, "sScriptname");
			lua_pushstring(L, (char*)li->mScriptName.c_str());
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

int _SQLQuery(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "SQLQuery", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	cpiLua *pi = (cpiLua*)serv->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);

	if (pi == NULL) {
		luaerror(L, ERR_PLUG);
		return 2;
	}

	pi->mQuery->Clear();
	pi->mQuery->OStream() << (char*)lua_tostring(L, 2);
	pi->mQuery->Query();
	int i = pi->mQuery->StoreResult();
	lua_pushboolean(L, 1);
	lua_pushnumber(L, i);
	return 1;
}

int _SQLFetch(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "SQLFetch", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isnumber(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	cpiLua *pi = (cpiLua*)serv->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);

	if (pi == NULL) {
		luaerror(L, ERR_PLUG);
		return 2;
	}

	if (!pi->mQuery->GetResult()) {
		luaerror(L, "No results");
		return 2;
	}

	int r = (int)lua_tonumber(L, 2);
	pi->mQuery->DataSeek(r);
	MYSQL_ROW row;

	if (!(row = pi->mQuery->Row())) {
		luaerror(L, "Error fetching row");
		return 2;
	}

	lua_pushboolean(L, 1);
	r = 0;

	while (r < pi->mQuery->Cols()) {
		lua_pushstring(L, (char*)row[r]);
		r++;
	}

	return 1;
}

int _SQLFree(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	cpiLua *pi = (cpiLua*)serv->mPluginManager.GetPlugin(LUA_PI_IDENTIFIER);

	if (pi == NULL) {
		luaerror(L, ERR_PLUG);
		return 2;
	}

	pi->mQuery->Clear();
	lua_pushboolean(L, 1);
	return 1;
}

int _AddRegUser(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "AddRegUser", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 1;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	string pass = (char*)lua_tostring(L, 3);
	int cls = 1;
	string op("");

	if (lua_isnumber(L, 4)) {
		cls = (int)lua_tonumber(L, 4);
	}

	if (lua_isstring(L, 5)) {
		op = (char*)lua_tostring(L, 5);
	}

	if (!AddRegUser((char*)nick.c_str(), cls, (char*)pass.c_str(), (char*)op.c_str())) {
		luaerror(L, "Unable to register new user");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _DelRegUser(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "DelRegUser", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 1;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);

	if (!DelRegUser((char*)nick.c_str())) {
		luaerror(L, "Registered user not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _GetTopic(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	lua_pushstring(L, serv->mC.hub_topic.c_str());
	return 1;
}

int _SetTopic(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "SetTopic", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string topic = (char*)lua_tostring(L, 2);
	string message;
	cDCProto::Create_HubName(message, serv->mC.hub_name, topic);
	serv->SendToAll(message, eUC_NORMUSER, eUC_MASTER);
	lua_pushboolean(L, 1);
	return 1;
}

int _IsUserOnline(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "IsUserOnline", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	cUser *usr = serv->mUserList.GetUserByNick(nick);
	lua_pushboolean(L, (usr == NULL) ? 0 : 1);
	return 1;
}

int _InUserSupports(lua_State *L)
{
	if (lua_gettop(L) < 3) {
		luaL_error(L, ERR_ARGS, "InUserSupports", 2, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	cUser *usr = serv->mUserList.GetUserByNick(nick);

	if ((usr == NULL) || (usr->mxConn == NULL)) {
		luaerror(L, "User not found");
		return 2;
	}

	string flag = lua_tostring(L, 3);

	if (
	((flag == "OpPlus") && (usr->mxConn->mFeatures & eSF_OPPLUS)) ||
	((flag == "NoHello") && (usr->mxConn->mFeatures & eSF_NOHELLO)) ||
	((flag == "NoGetINFO") && (usr->mxConn->mFeatures & eSF_NOGETINFO)) ||
	((flag == "DHT0") && (usr->mxConn->mFeatures & eSF_DHT0)) ||
	((flag == "QuickList") && (usr->mxConn->mFeatures & eSF_QUICKLIST)) ||
	((flag == "BotINFO") && (usr->mxConn->mFeatures & eSF_BOTINFO)) ||
	(((flag == "ZPipe0") || (flag == "ZPipe")) && (usr->mxConn->mFeatures & eSF_ZLIB)) ||
	((flag == "ChatOnly") && (usr->mxConn->mFeatures & eSF_CHATONLY)) ||
	((flag == "MCTo") && (usr->mxConn->mFeatures & eSF_MCTO)) ||
	((flag == "UserCommand") && (usr->mxConn->mFeatures & eSF_USERCOMMAND)) ||
	((flag == "BotList") && (usr->mxConn->mFeatures & eSF_BOTLIST)) ||
	((flag == "HubTopic") && (usr->mxConn->mFeatures & eSF_HUBTOPIC)) ||
	((flag == "UserIP2") && (usr->mxConn->mFeatures & eSF_USERIP2)) ||
	((flag == "TTHSearch") && (usr->mxConn->mFeatures & eSF_TTHSEARCH)) ||
	((flag == "Feed") && (usr->mxConn->mFeatures & eSF_FEED)) ||
	((flag == "ClientID") && (usr->mxConn->mFeatures & eSF_CLIENTID)) ||
	((flag == "IN") && (usr->mxConn->mFeatures & eSF_IN)) ||
	((flag == "BanMsg") && (usr->mxConn->mFeatures & eSF_BANMSG)) ||
	((flag == "TLS") && (usr->mxConn->mFeatures & eSF_TLS))
	) {
		lua_pushboolean(L, 1);
		lua_pushboolean(L, 1);
		return 1;
	}

	lua_pushboolean(L, 1);
	lua_pushboolean(L, 0);
	return 1;
}

int _GetNickList(lua_State *L)
{
	int res = 1;
	char *nl = GetNickList();
	if (strlen(nl) < 1) res = 0;
	lua_pushboolean(L, res);
	lua_pushstring(L, nl);
	return 1;
}

int _GetOPList(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	int res = 1;
	char *ol = (char*)serv->mOpList.GetNickList().c_str();
	if (strlen(ol) < 1) res = 0;
	lua_pushboolean(L, res);
	lua_pushstring(L, ol);
	return 1;
}

int _GetBotList(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	int res = 1;
	char *bl = (char*)serv->mRobotList.GetNickList().c_str(); // @fixme: mRobotList is empty
	if (strlen(bl) < 1) res = 0;
	lua_pushboolean(L, res);
	lua_pushstring(L, bl);
	return 1;
}

int _StopHub(lua_State *L)
{
	if (lua_gettop(L) < 2) {
		luaL_error(L, ERR_ARGS, "StopHub", 1, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isnumber(L, 2)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	int code = (int)lua_tonumber(L, 2);

	if (!StopHub(code)) {
		luaerror(L, ERR_CALL);
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _GetVHCfgDir(lua_State *L)
{
	lua_pushboolean(L, 1);
	lua_pushstring(L, GetVHCfgDir());
	return 1;
}

int _GetUpTime(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	int sf = 1;

	if (lua_gettop(L) > 1) {
		if (!lua_isnumber(L, 2)) {
			luaerror(L, ERR_PARAM);
			return 2;
		}

		if ((int)lua_tonumber(L, 2) == 2) sf = 2;
	}

	cTime upTime;
	upTime = serv->mTime;
	upTime -= serv->mStartTime;
	lua_pushboolean(L, 1);

	if (sf == 1)
		lua_pushnumber(L, upTime.Sec());
	else
		lua_pushnumber(L, upTime.MiliSec());

	return 1;
}

int _GetHubIp(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	lua_pushboolean(L, 1);
	lua_pushstring(L, (char*)serv->mAddr.c_str());
	return 1;
}

int _GetHubSecAlias(lua_State *L)
{
	cServerDC *serv = GetServ();

	if (serv == NULL) {
		luaerror(L, ERR_SERV);
		return 2;
	}

	lua_pushboolean(L, 1);
	lua_pushstring(L, (char*)serv->mC.hub_security.c_str());
	return 1;
}

int _GetUsersCount(lua_State *L)
{
	lua_pushboolean(L, 1);
	lua_pushnumber(L, GetUsersCount());
	return 1;
}

int _GetTotalShareSize(lua_State *L)
{
	lua_pushboolean(L, 1);
	lua_pushnumber(L, GetTotalShareSize());
	return 1;
}

int _ParseCommand(lua_State *L)
{
	if (lua_gettop(L) < 4) {
		luaL_error(L, ERR_ARGS, "ParseCommand", 3, lua_gettop(L) - 1);
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		return 2;
	}

	if (!lua_isstring(L, 2) || !lua_isstring(L, 3) || !lua_isnumber(L, 4)) {
		luaerror(L, ERR_PARAM);
		return 2;
	}

	string nick = (char*)lua_tostring(L, 2);
	string cmd = (char*)lua_tostring(L, 3);
	int pm = (int)lua_tonumber(L, 4);

	if (!ParseCommand((char*)nick.c_str(), (char*)cmd.c_str(), pm)) {
		luaerror(L, "User not found");
		return 2;
	}

	lua_pushboolean(L, 1);
	return 1;
}

int _SetTempRights(lua_State *L)
{
	luaL_error(L, "VH:SetTempRights not implemented yet.");
	return 2;
}

int _GetTempRights(lua_State *L)
{
	luaL_error(L, "VH:GetTempRights not implemented yet.");
	return 2;
}

void luaerror(lua_State *L, const char *errstr)
{
	lua_pushboolean(L, 0);
	lua_pushstring(L, errstr);
}

cLuaInterpreter *FindLua(lua_State *L)
{
	cLuaInterpreter *temp;
	int vSize = cpiLua::me->Size();

	for (int i = 0; i < vSize; i++) {
		temp = cpiLua::me->mLua[i];
		if (temp->mL == L) return temp;
	}

	return NULL;
}

}; // namespace nVerliHub
