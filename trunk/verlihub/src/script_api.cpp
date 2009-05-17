/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include <cserverdc.h>
#include <cban.h>
#include <cbanlist.h>
#include <creglist.h>
#include "script_api.h"
#include "cconfigitembase.h"

using namespace std;
using namespace nDirectConnect;

cServerDC *GetCurrentVerlihub()
{
	return (cServerDC *)cServerDC::sCurrentServer;
}

cUser *GetUser(char *nick)
{
	cServerDC *server = GetCurrentVerlihub();
	if(!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return NULL;
	}
	cUser *usr = server->mUserList.GetUserByNick(string(nick));
	
	//user without connection (bot) must be accepted as well
	if (usr == NULL)
		return NULL;
	
	return usr;
}

bool SendDataToUser(char *data, char *nick)
{
	cUser *usr = GetUser(nick);
	if (!usr)
		return false;
	else if	(!usr->mxConn) {
		
		return false;
	} else {
		string omsg(data);
		usr->mxConn->Send(omsg, true);
	}
	return true;
}

bool KickUser(char *OP,char *nick, char *reason)
{
	cServerDC *server = GetCurrentVerlihub();
	cUser *OPusr = GetUser(OP);
	ostringstream os;
	if(OPusr)
	{
		if(server)
		{
			server->DCKickNick(&os, OPusr, nick, reason, cServerDC::eKCK_Drop|cServerDC::eKCK_Reason|cServerDC::eKCK_PM|cServerDC::eKCK_TBAN);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool SendToClass(char *data, int min_class,  int max_class)
{
	cServerDC *server = GetCurrentVerlihub();
	if(!server) {
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	if(min_class > max_class) return false;
	string omsg(data);
	server->mUserList.SendToAllWithClass(omsg, min_class, max_class,false, false);
	return true;
}

bool SendToAll(char *data)
{
	cServerDC *server = GetCurrentVerlihub();
	if(!server) {
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	string omsg(data);
	server->mUserList.SendToAll(omsg);
	return true;
}


bool SendPMToAll(char *data, char *from, int min_class,  int max_class)
{
	string start, end;
	cServerDC *server = GetCurrentVerlihub();
	if (!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	server->mP.Create_PMForBroadcast(start, end, from, from, data);
	server->SendToAllWithNick(start,end, min_class, max_class);
	return true;
}

bool CloseConnection(char *nick)
{
	cUser *usr = GetUser(nick);
	if((!usr) || (usr && !usr->mxConn))
		return false;
	else
	{
		usr->mxConn->CloseNow();
		return true;
	}
}

char * GetUserCC(char * nick)
{
	cUser *usr = GetUser(nick);
	if((!usr) || (usr && !usr->mxConn))
		return NULL;
	else {
		return (char *) usr->mxConn->mCC.c_str();
	}
	
}

char *GetMyINFO(char *nick)
{
	cUser *usr = GetUser(nick);
	if(usr) return (char*) usr->mMyINFO.c_str();
	else return (char *)"";
}

int GetUserClass(char * nick)
{
	cUser *usr = GetUser(nick);
	if(usr) return usr->mClass;
	else return -1;
}

char *GetUserHost(char *nick)
{
	cUser *usr = GetUser(nick);
	if ((!usr) || (usr && !usr->mxConn))
		return (char *)"";
	else
	{
		cServerDC *server = GetCurrentVerlihub();
		if ( server == NULL) {
			cerr << "Server verlihub is unfortunately not running or not found." << endl;
			return (char *)"";
		}
		if(!server->mUseDNS)
			usr->mxConn->DNSLookup();
		return (char*)usr->mxConn->AddrHost().c_str();
	}
}

char *GetUserIP(char *nick)
{
	cUser *usr = GetUser(nick);
	if ((!usr) || (usr && !usr->mxConn))
		return (char *)"";
	else
	{
		return (char*)usr->mxConn->AddrIP().c_str();
	}
}

bool Ban(char *nick, const string op, const string reason, unsigned howlong, unsigned bantype)
{
	cServerDC *server = GetCurrentVerlihub();
	if(!server) {
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	cUser *usr = GetUser(nick);
	if ((!usr) || (usr && !usr->mxConn)) return false;
	
	cBan ban(server);
	server->mBanList->NewBan(ban, usr->mxConn, op, reason, howlong, bantype);
	server->mBanList->AddBan(ban);
	usr->mxConn->CloseNice(1000, eCR_KICKED);
	return true;
}

char * ParseCommand(char *command_line)
{
	cServerDC *server = GetCurrentVerlihub();
	if(!server) {
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	cUser *usr = GetUser((char *) server->mC.hub_security.c_str());
	printf("%p\n", usr);
	printf("%p", usr->mxConn);
	if ((!usr) || (usr && !usr->mxConn)) return false;
	cout << "here" << endl;
	if (!server->mP.ParseForCommands(command_line, usr->mxConn)) {
		// unknown command	
	}
	return (char *) "";
}

bool SetConfig(char *config_name, char *var, char *val)
{
	// config_name ignored for now!
	cServerDC *server = GetCurrentVerlihub();
	if(!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	
	string file(server->mDBConf.config_name);
		
	cConfigItemBase *ci = NULL;
	if(file == server->mDBConf.config_name)
	{
		ci = server->mC[var];
		if( !ci )
		{
		cerr << "Undefined variable: " << var << endl;
		return false;
		}
	}
	if(ci)
	{
		ci->ConvertFrom(val);
		server->mSetupList.SaveItem(file.c_str(), ci);
	}
	return true;
}

int GetConfig(char *config_name, char *var, char *buf, int size)
{
	// config_name ignored for now!
	cServerDC *server = GetCurrentVerlihub();
	if(!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return -1;
	}
	
	if (size < 1) return -1;
	buf[0] = 0;
	
	string val;
	string file(server->mDBConf.config_name);
	
	cConfigItemBase *ci = NULL;
	if(file == server->mDBConf.config_name)
	{
		ci = server->mC[var];
		if(!ci)
		{
			cerr << "Undefined variable: " << var << endl;
			return -1;
		}
	}
	
	if(ci)
	{
		ci->ConvertTo(val);
		if(!val.size()) return 0;
		if(int(val.size()) < size)
		{
			memcpy(buf, val.data(), val.size());
			buf[val.size()] = 0;
		}
		return val.size();
	}
	
	return -1;
}

int __GetUsersCount()
{
	cServerDC *server = GetCurrentVerlihub();
	if (!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return 0;
	}
	return server->mUserCountTot;
}

		
__int64 GetTotalShareSize()
{
	cServerDC *server = GetCurrentVerlihub();
	if (!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return 0;
	}
	return server->GetTotalShareSize();
}

char *__GetNickList()
{
	cServerDC *server = GetCurrentVerlihub();
	if(server)
	{
		return (char*)server->mUserList.GetNickList().c_str();
	} else return (char *) "";
}

char * GetVHCfgDir()
{
	cServerDC *server = GetCurrentVerlihub();
	if (!server) {
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return 0;
	}
	return (char *) server->mConfigBaseDir.c_str();
}

bool GetTempRights(char *nick,  map<string,int> &rights)
{
	cUser *user = GetUser(nick);
	if(user == NULL) return false;
	cTime time = cTime().Sec();

	static const int ids[] = { nDirectConnect::nEnums::eUR_CHAT, nDirectConnect::nEnums::eUR_PM, nDirectConnect::nEnums::eUR_SEARCH, nDirectConnect::nEnums::eUR_CTM, nDirectConnect::nEnums::eUR_KICK, nDirectConnect::nEnums::eUR_REG, nDirectConnect::nEnums::eUR_OPCHAT, nDirectConnect::nEnums::eUR_DROP, nDirectConnect::nEnums::eUR_TBAN, nDirectConnect::nEnums::eUR_PBAN, nDirectConnect::nEnums::eUR_NOSHARE };
	for(int i = 0; i< sizeof ids; i++) {
		string key;
		switch(ids[i]) {
			case nDirectConnect::nEnums::eUR_CHAT:
				key = "mainchat";
			break;
			case nDirectConnect::nEnums::eUR_PM:
				key = "pm";
			break;
			case nDirectConnect::nEnums::eUR_SEARCH:
				key = "search";
			break;
			case nDirectConnect::nEnums::eUR_CTM:
				key = "ctm";
			break;
			case nDirectConnect::nEnums::eUR_KICK:
				key = "kick";
			break;
			case nDirectConnect::nEnums::eUR_REG:
				key = "reg";
			break;
			case nDirectConnect::nEnums::eUR_OPCHAT:
				key = "opchat";
			break;
			case nDirectConnect::nEnums::eUR_DROP:
				key = "drop";
			break;
			case nDirectConnect::nEnums::eUR_TBAN:
				key = "tempban";
			break;
			case nDirectConnect::nEnums::eUR_PBAN:
				key = "perban";
			break;
			case nDirectConnect::nEnums::eUR_NOSHARE:
				key = "noshare";
			break;
		}
		if(!key.empty()) rights[key] = (user->Can(ids[i],time) ? 1 : 0);
	}
	return true;
}

bool AddRegUser(char *nick, int uClass, char * passwd, char* op)
{
	cServerDC *server = GetCurrentVerlihub();
	if(server == NULL) {
		  cerr << "Server verlihub is not running or not found." << endl;
		  return false;
	}
		
	cConnDC * conn = NULL;
	
	if(strlen(op) > 0) {
		cUser *user = GetUser(op);
		if(user && user->mxConn) conn = user->mxConn;
	}
	if(uClass == eUC_MASTER) return false;
	if( strlen(passwd) < server->mC.password_min_len ) return false;
	return server->mR->AddRegUser(nick, conn, uClass, passwd);
}
bool DelRegUser(char *nick)
{
	cServerDC *server = GetCurrentVerlihub();
	if(server == NULL) {
		cerr << "Server verlihub is not running or not found." << endl;
		return false;
	}
	
	cRegUserInfo ui;
	bool RegFound = server->mR->FindRegInfo(ui, nick);
	if(!RegFound) return false;
	if(ui.mClass == eUC_MASTER) return false;
	return server->mR->DelReg(nick);
}
extern "C" { 
	int GetUsersCount()
	{
		return __GetUsersCount();
	}
	char *GetNickList()
	{
		return __GetNickList();
	}
}
	
