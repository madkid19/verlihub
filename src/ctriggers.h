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
#ifndef NDIRECTCONNECTCTRIGGERS_H
#define NDIRECTCONNECTCTRIGGERS_H
#include "ctrigger.h"
#include "tmysqlmemorylist.h"
#include "tlistconsole.h"
#include "cdcconsole.h"

using nConfig::tMySQLMemoryList;

namespace  {

class cServerDC;
class cDCConsole;
namespace nTables {

typedef tMySQLMemoryList<cTrigger, cServerDC> tTriggersBase;

  /**
  This class contains the list of all triggers and is able to add, edit and delete them

  @author Daniel Muller
  @author Simoncelli Davide
  
  */
class cTriggers : public tTriggersBase
{
public:
	cTriggers(cServerDC *);
	virtual ~cTriggers(){};
	virtual void AddFields();
	cTrigger * Find(const string &name); //@todo Trigger stuff
	virtual bool CompareDataKey(const cTrigger &D1, const cTrigger &D2);
	void OnTimer(long now);
	// useful functions
	void TriggerAll(int FlagMask, cConnDC *conn);
	bool DoCommand(cConnDC *conn, const string &cmd, istringstream &cmd_line, cServerDC &server);
};


typedef tListConsole<cTrigger, cTriggers, cDCConsole> tTriggerConsoleBase;

  /**

  Trigger console to manage triggers

  @author Daniel Muller
  @author Simoncelli Davide

  */

class cTriggerConsole: public tTriggerConsoleBase
{
public:
	cTriggerConsole(cDCConsole *console);
	virtual ~cTriggerConsole();
	virtual const char * GetParamsRegex(int cmd);
	virtual cTriggers *GetTheList();
	virtual const char *CmdSuffix();
	virtual const char *CmdPrefix();
	virtual void ListHead(ostream *os);
	virtual bool IsConnAllowed(cConnDC *conn,int cmd);
	virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cTrigger &data);
	virtual void GetHelpForCommand(int cmd, ostream &os);
	virtual void GetHelp(ostream &os);

	bool CheckData(cfBase *cmd, cTrigger &tmp);

};


};
};

#endif
