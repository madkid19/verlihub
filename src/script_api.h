/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
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
#ifndef _WIN32
#define __int64 long long
#endif
#include <map>
#include <string>

using namespace std;
namespace nVerliHub {
	bool SendDataToUser(char *data, char *nick);
	bool SendToClass(char *data, int min_class,  int max_class);
	bool SendToAll(char *data);
	bool SendPMToAll(char *data, char *from, int min_class,  int max_class);
	bool CloseConnection(char *nick);
	bool StopHub(int code);
	char * GetUserCC(char * nick);
	string GetIPCC(const string);
	char * GetMyINFO(char *nick);
	int GetUserClass(char *nick);
	char * GetUserHost(char *nick);
	char * GetUserIP(char *nick);
	bool Ban(char *, const string, const string, unsigned, unsigned);
	bool ParseCommand(char *nick, char *cmd, int pm);
	bool KickUser(char *OPNick, char *nick, char *reason);
	bool SetConfig(char *config_name, char *var, char *val);
	int GetConfig(char *config_name, char *var, char *val, int size);
	char * GetVHCfgDir();
	bool GetTempRights(char *nick,  map<string,int> &rights);
	bool AddRegUser(char *nick, int uClass, char * passwd, char* op);
	bool DelRegUser(char *nick);
	extern "C" {
		int GetUsersCount();
		char * GetNickList();

	}
	__int64 GetTotalShareSize();
};
