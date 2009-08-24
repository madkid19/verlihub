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
#ifndef NDIRECTCONNECT_NTABLESCDCCLIENTS_H
#define NDIRECTCONNECT_NTABLESCDCCLIENTS_H
#include "tmysqlmemorylist.h"
#include "tlistconsole.h"

using namespace nConfig;
namespace nDirectConnect
{

class cServerDC;
class cDCConsole;

namespace nTables
{


class cDCClient
{
public:
	cDCClient();
	virtual ~cDCClient(){};
	string mName;
	string mPrefixRegex; // the thing in front of the tag	
	string mInTagID; // the first part of the tag
	int mIncertitude;
	int mPrefixVersionRank;
	bool mLimitIsPerSlot;
	double mMinVersion;
	double mMaxVersion;
	// Overriding and needed methods
	virtual void OnLoad();
	friend ostream &operator << (ostream &, cDCClient &);

	// custom methods
	bool CheckIt(const string &prefix, const string &InTagID, double &Version);
};

typedef tMySQLMemoryList<cDCClient, cServerDC> tDCClientsBase;

/**
@author Daniel Muller
*/
class cDCClients : public tDCClientsBase
{
public:
	cDCClients(cServerDC *server);
	virtual ~cDCClients();
	
	// overiding methods
	virtual void AddFields();
	virtual bool CompareDataKey(const cDCClient &D1, const cDCClient &D2);
};

typedef tListConsole<cDCClient, cDCClients, cDCConsole> tDCClientConsoleBase;

class cDCClientConsole: public tDCClientConsoleBase
{
public:
	cDCClientConsole(cDCConsole *console);
	virtual ~cDCClientConsole();
	virtual const char * GetParamsRegex(int cmd);
	virtual cDCClients *GetTheList();
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual void ListHead(ostream *os);
	virtual bool IsConnAllowed(cConnDC *conn,int cmd);
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cDCClient &data);
};

};

};

#endif
