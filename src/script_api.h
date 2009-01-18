/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _WIN32
#define __int64 long long
#endif

bool SendDataToUser(char *data, char *nick);
bool SendToClass(char *data, int min_class,  int max_class);
bool SendToAll(char *data);
bool SendPMToAll(char *data, char *from, int min_class,  int max_class);
bool CloseConnection(char *nick);
char * GetUserCC(char * nick);
char * GetMyINFO(char *nick);
int GetUserClass(char *nick);
char * GetUserHost(char *nick);
char * GetUserIP(char *nick);
bool Ban(char *, const string, const string, unsigned, unsigned);
char * ParseCommand(char *command_line);
bool KickUser(char *OPNick, char *nick, char *reason);
bool SetConfig(char *config_name, char *var, char *val);
int GetConfig(char *config_name, char *var, char *val, int size);
char * GetVHCfgDir();
bool GetTempRights(char *, int *);
bool AddRegUser(char *nick, int uClass, char * passwd, char* op);
bool DelRegUser(char *nick);
extern "C" { 
	int GetUsersCount(); 
	char * GetNickList();

}
__int64 GetTotalShareSize();
