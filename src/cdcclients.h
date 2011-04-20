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
#ifndef NDIRECTCONNECTDCCLIENTS_H
#define NDIRECTCONNECTDCCLIENTS_H
#include "tmysqlmemorylist.h"
#include "tlistconsole.h"
#include "cdcconsole.h"
#include <sstream>
#include "cpcre.h"
#include "cdctag.h"
#include "cdcclient.h"

using namespace std;
namespace nVerliHub {
	//using namespace nTables;
	//namespace nUtils { class cPCRE; };
	//using nConfig::tMySQLMemoryList;

	namespace nEnums {
		// TODO: Remove ME!
		typedef enum {
			eCT_NOTAG,
			eCT_PLUSPLUS,
			eCT_DCGUI,
			eCT_ODC,
			eCT_DC,
			eCT_DCPRO,
			eCT_STRONGDC,
			eCT_IDC, eCT_ZDC,
			eCT_APEXDC,
			eCT_ZION,
			eCT_UNKNOWN
		} tClientType;
	};

	/**
	* Parser for DC Tag
	* @author Daniel Muller, Simoncelli Davide
	*/
	class cDCTagParser
	{
		public:
		// the constructor
		cDCTagParser();
		/** the global teg's regular expression */
		nUtils::cPCRE mTagRE;
		nUtils::cPCRE mModeRE;
		nUtils::cPCRE mHubsRE;
		nUtils::cPCRE mSlotsRE;
		nUtils::cPCRE mLimitRE;
	};

	class cServerDC;
	class cDCConsole;
	namespace nTables {

		typedef tMySQLMemoryList<cDCClient, cServerDC> tClientsBase;
		class cDCClients : public tClientsBase
		{
			static cDCTagParser mParser;
			cServerDC *mServer;
			public:
				int mPositionInDesc;

				cDCClients(cServerDC *);
				virtual ~cDCClients(){};
				virtual void AddFields();;
				cDCClient* FindTag(const string &tagID);
				virtual bool CompareDataKey(const cDCClient &D1, const cDCClient &D2);
				bool ParsePos(const string &desc);
				cDCTag* ParseTag(const string &desc);
				bool ValidateTag(cDCTag *tag, ostream &os, cConnType *conn_type, int &code);
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
				virtual bool IsConnAllowed(nSocket::cConnDC *conn,int cmd);
				virtual bool ReadDataFromCmd(cfBase *cmd, int CmdID, cDCClient &data);
				virtual void GetHelpForCommand(int cmd, ostream &os);
				virtual void GetHelp(ostream &os);

		};
	}; // namespace nTables
}; // namespace nVerliHub

#endif

