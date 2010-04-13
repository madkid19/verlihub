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
#ifndef NDIRECTCONNECT_NTABLESCCONNTYPES_H
#define NDIRECTCONNECT_NTABLESCCONNTYPES_H
#include "tmysqlmemorylist.h"
#include "tlistconsole.h"

using namespace nConfig;
namespace nDirectConnect
{

class cServerDC;
class cDCConsole;

namespace nTables
{

class cConnType
{
public:
	cConnType();
	virtual ~cConnType ();

	// database variables
	string mIdentifier;
	string mDescription;
	int mTagMaxSlots;
	int mTagMinSlots;
	double mTagMinLimit;
	double mTagMinLSRatio;
	
	// Overriding and needed methods
	virtual void OnLoad();
	friend ostream &operator << (ostream &, cConnType &);
};


typedef tMySQLMemoryList<cConnType, cServerDC> tConnTypesBase;

/**
Table for connection types and their configuration
 
@author Daniel Muller
*/

class cConnTypes : public tConnTypesBase
{
public:
	cConnTypes(cServerDC *server);
	virtual ~cConnTypes();
	// overiding methods
	virtual void AddFields();
	virtual bool CompareDataKey(const cConnType &D1, const cConnType &D2);
	
	//custom methods
	cConnType *FindConnType(const string &identifier);
};

typedef tListConsole<cConnType, cConnTypes, cDCConsole> tConnTypeConsoleBase;

class cConnTypeConsole: public tConnTypeConsoleBase
{
public:
	cConnTypeConsole(cDCConsole *console);
	virtual ~cConnTypeConsole();
	virtual const char * GetParamsRegex(int cmd);
	virtual cConnTypes *GetTheList();
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual void ListHead(ostream *os);
	virtual bool IsConnAllowed(cConnDC *conn,int cmd);
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cConnType &data);
	virtual void GetHelpForCommand(int cmd, ostream &os);
};
};

};

#endif
