/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include <cserverdc.h>
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

bool SendDataToAll(char *data, int min_class,  int max_class)
{
	cServerDC *server = GetCurrentVerlihub();
	if(!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return false;
	}
	if(min_class > max_class)
		return false;
	string omsg(data);
	if (min_class >= 0 && min_class < 3)
		server->mUserList.SendToAll(omsg,true, false);
	else if(min_class >= 3)
		server->mOpchatList.SendToAll(omsg,true, false);
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
	server->LastBCNick = from;
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

bool Ban(char *nick, unsigned howlong, int bantype)
{
	return false;
}

char * ParseCommand(char *command_line)
{
	return "";
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
	} else return "";
}

char * GetVerliPath()
{
	cServerDC *server = GetCurrentVerlihub();
	if (!server)
	{
		cerr << "Server verlihub is unfortunately not running or not found." << endl;
		return 0;
	}
	return (char*)server->mConfigBaseDir.c_str();
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
	
