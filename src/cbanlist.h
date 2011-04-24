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

#ifndef NDIRECTCONNECTCBANLIST_H
#define NDIRECTCONNECTCBANLIST_H
#include "cconfmysql.h"
#include "cban.h"
#include "ckick.h"
#include <string>
#include <iostream>
#include "thasharray.h"

using std::string;
using std::ostream;

namespace nVerliHub {

	namespace nSocket {
		class cConnDC;
	};

	namespace nTables {

		class cUnBanList;

/** \brief a Banlist Manager
  *
  * a mysql based banlist, allows to check for ban
  * add ban
  * remove ban
  * get a list of bans
  * etc..
  * @author Daniel Muller
  */
class cBanList : public nConfig::cConfMySQL
{
	friend class nVerliHub::nSocket::cServerDC;
	struct sTempBan
	{
		sTempBan(long u, const string &reason):mUntil(u), mReason(reason){};
		long mUntil;
		string mReason;
	};
	public:
		cBanList(nSocket::cServerDC*);
		~cBanList();
		int UpdateBan(cBan &);
		bool LoadBanByKey(cBan &ban);
		void NewBan(cBan &,nSocket::cConnDC *,const string &nick_op, const string &reason,
			unsigned length, unsigned type);
		void AddBan(cBan &);
		bool TestBan(cBan &, nSocket::cConnDC *, const string &, unsigned mask);
		virtual void Cleanup();
		void DelBan(cBan &);
		int DeleteAllBansBy(const string &IP, const string &Nick, int Flags);
		void NewBan(cBan &Dest, const cKick &Kick, long period, int Type);
		// Remove ban for given ip nick or whatever.. and also corresponding bans.
		int Unban(ostream &os, const string &What, const string &reason, const string &NickOp, int TypeOfWhat, bool DoIt = true);
		/**
		* Extract the host (of given level) substring from a given string
		*/
		bool GetHostSubstring(const string &src, string &dest, int level);
		bool AddTestCondition(ostream &os, const string &What, int Type);
		void SetUnBanList(cUnBanList *UnBanList){mUnBanList = UnBanList;}
		void List(ostream &os, int count);
		static unsigned long Ip2Num(const string &ip);
		static void Num2Ip(unsigned long num, string &ip);
		long IsIPTempBanned(unsigned long ip);
		void AddIPTempBan(unsigned long ip, long until, const string &reason);
		void DelIPTempBan(unsigned long);
		long IsNickTempBanned(const string &nick);
		void AddNickTempBan(const string &nick, long until, const string &reason);
		void DelNickTempBan(const string &nick);
		int RemoveOldShortTempBans(long before);
	private:
		cUnBanList *mUnBanList;
		typedef tHashArray<sTempBan *> tTempNickBans;
		tTempNickBans mTempNickBanlist;
		tTempNickBans mTempIPBanlist;
		nSocket::cServerDC* mS;
	protected:
		cBan mModel;
};

/**
The list of unbans, keeps trace of bans removed by ops
Banlist uses this class to produce unbans
@author Daniel Muller
*/
class cUnBanList : public cBanList
{
	public:
		cUnBanList(nSocket::cServerDC*);
		~cUnBanList();
		virtual void Cleanup();
	protected:
		cUnBan mModelUn;
};

};
};

#endif
