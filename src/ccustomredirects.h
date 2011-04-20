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
#ifndef NDIRECTCONNECTCREDIRECTS_H
#define NDIRECTCONNECTCREDIRECTS_H
#include "ccustomredirect.h"
#include "tmysqlmemorylist.h"
#include "tlistconsole.h"
#include "cdcconsole.h"

namespace nVerliHub {
	using nConfig::tMySQLMemoryList;
	class cServerDC;
	class cDCConsole;
	namespace nTables {

		typedef tMySQLMemoryList<cRedirect, cServerDC> tRedirectsBase;
		class cRedirects : public tRedirectsBase
		{
			public:
				cRedirects(cServerDC *);
				virtual ~cRedirects(){};
				virtual void AddFields();
				cRedirect * Find(const string &name); //@todo Trigger stuff
				virtual bool CompareDataKey(const cRedirect &D1, const cRedirect &D2);
				char *MatchByType(int Type);
				int MapTo(int Type);
				void Random(int &key);
		};


		typedef tListConsole<cRedirect, cRedirects, cDCConsole> tRedirectConsoleBase;

		class cRedirectConsole: public tRedirectConsoleBase
		{
			public:
				cRedirectConsole(cDCConsole *console);
				virtual ~cRedirectConsole();
				virtual const char * GetParamsRegex(int cmd);
				virtual cRedirects *GetTheList();
				virtual const char *CmdSuffix();
				virtual const char *CmdPrefix();
				virtual void ListHead(ostream *os);
				virtual bool IsConnAllowed(cConnDC *conn,int cmd);
				virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cRedirect &data);
				virtual void GetHelpForCommand(int cmd, ostream &os);
				virtual void GetHelp(ostream &os);

		};
	}; // namespace nTables
}; // namespace nVerliHub

#endif
