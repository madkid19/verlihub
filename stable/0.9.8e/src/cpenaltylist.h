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

#ifndef NDIRECTCONNECT_NTABLESCPENALTYLIST_H
#define NDIRECTCONNECT_NTABLESCPENALTYLIST_H

#include <string>
#include "cconfmysql.h"
#include "ctime.h"
#include "tcache.h"

using std::string;
using nUtils::cTime;
using nConfig::cConfMySQL;
using namespace nUtils;

namespace nDirectConnect {

namespace nTables {

/**
has the list of temporary user penalties... that save in database

@author Daniel Muller
*/

class cPenaltyList : public cConfMySQL
{
public:
	/**structure representing the MySQL table data */
	struct sPenalty
	{
		string mNick;
		long mSince;
		long mStartChat;
		long mStartSearch;
		long mStartCTM;
		long mStartPM;
		long mStopKick;
		long mStopShare0;
		long mStopReg;
		long mStopOpchat;

		sPenalty()
		{
			long Now = cTime().Sec();
			mSince = Now;
			mStartChat = 1;
			mStartSearch = 1;
			mStartCTM = 1;
			mStartPM = 1;
			mStopKick = 1;
			mStopShare0 = 1;
			mStopReg = 1;
			mStopOpchat = 1;
		}

		bool ToKeepIt()
		{
			long Now = cTime().Sec();
			if (mStartChat >  Now) return true;
			if (mStartSearch > Now) return true;
			if (mStartCTM > Now) return true;
			if (mStartPM > Now) return true;
			if (mStopKick > Now) return true;
			if (mStopShare0 > Now) return true;
			if (mStopReg > Now) return true;
			if (mStopOpchat > Now) return true;
			return false;
		}
		friend ostream &operator << (ostream &, const sPenalty &);
	};


	cPenaltyList(cMySQL &mysql);
	~cPenaltyList();
	void Cleanup(void);
	bool LoadTo(sPenalty &, const string &Nick);
	bool AddPenalty(sPenalty &);
	bool RemPenalty(sPenalty &);
	void ReloadCache(){mCache.Clear(); mCache.LoadAll();}
	tCache<string> mCache;
protected:
	sPenalty mModel;
};

};

};

#endif
