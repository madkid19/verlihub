/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NDIRECTCONNECTCREDIRECTS_H
#define NDIRECTCONNECTCREDIRECTS_H

#include "ccustomredirect.h"
#include "tmysqlmemorylist.h"
#include "tlistconsole.h"
#include "cdcconsole.h"

using nConfig::tMySQLMemoryList;

namespace nDirectConnect {
	
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


	};
};

#endif
 
