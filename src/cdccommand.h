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
#ifndef CDCCOMMAND_H
#define CDCCOMMAND_H
#include "ccommand.h"
#include "cobj.h"
using namespace ::nCmdr;

namespace nDirectConnect
{
	class cUser;
	class cServerDC;
	class cConnDC;

	template<class OwnerType>
	class tConsoleBase: public cObj
	{
		public:
		tConsoleBase(OwnerType *s) :
			cObj("nDC::Console"),
			mOwner(s)
			{}
		OwnerType *mOwner;
		virtual ~tConsoleBase(){}
		virtual int OpCommand(const string &, cConnDC*) = 0;
		virtual int UsrCommand(const string & , cConnDC * ) = 0;		
	};
	
	typedef tConsoleBase<cServerDC> cDCConsoleBase;

	class cDCCommand : public ::nCmdr::cCommand
	{
	public:
		cDCCommand();
		virtual ~cDCCommand(){};

		long mActionType;

		class sDCCmdFunc : public ::nCmdr::cCommand::sCmdFunc
		{
		public:
			cServerDC *mS;
			cConnDC * mConn;
			cDCConsoleBase *mCo;

			virtual ~sDCCmdFunc(){};
			virtual bool GetIDEnum(int rank, int &id, const char *ids[], const int enums[]);
			virtual bool GetParUnEscapeStr(int rank, string &dest);
			virtual bool GetParIPRange(int rank, unsigned long &ipmin, unsigned long &ipmax);
			virtual bool GetParRegex(int rank, string &dest);
			virtual bool GetParOnlineUser(int rank, cUser *&dest, string &nick);
			void Bind(cDCConsoleBase *co);
			virtual bool operator() (cPCRE &idrex, cPCRE &parrex, ostream &os, void *extra);
		};

		cDCCommand(int ID, const char *IdRegex, const char *ParRegex, sDCCmdFunc *CmdFunc, long Action = -1);
		virtual void Init(void *co);
		virtual void Init(int ID, const char *IdRegex, const char *, sCmdFunc*);
	};
};

#endif
