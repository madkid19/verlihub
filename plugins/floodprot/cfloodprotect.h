/***************************************************************************
 *   Copyright (C) 2004 by Janos Horvath                                   *
 *   bourne@freemail.hu                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _CFLOODPROTECT_H_
#define _CFLOODPROTECT_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "src/tchashlistmap.h"
#include "src/thasharray.h"
#include "src/ctime.h"
#include "src/cconfigbase.h"

#include <vector>

using namespace nUtils;
using namespace nConfig;

namespace nDirectConnect {

	class cConnDC;
	class cServerDC;

namespace nTables {

typedef enum { eFT_CHAT, eFT_PRIVATE, eFT_SEARCH, eFT_MYINFO } tFloodType;

struct sUserInfo
{
	cTime mLastAction;
	cTime mElapsedTime;
	unsigned short mActionCounter;
	string mIP;
	string mFTStr;
	bool mDisabled;
	list<tFloodType> mFloodTypes;

	sUserInfo(string ip) : mIP(ip), mActionCounter(0), mDisabled(false) {}
	~sUserInfo() {}
	void addFloodType(tFloodType ft)
	{
	    if(mFloodTypes.size() == 10)
	    {
		mFloodTypes.pop_front(); // drop the first
	    }
	    mFloodTypes.push_back(ft);
	}
	string & getFloodTypes()
	{
	    mFTStr.clear();
	    for(list<tFloodType>::iterator it=mFloodTypes.begin(); it!=mFloodTypes.end(); ++it)
	    {
		switch(*it)
		{
		    case eFT_CHAT: mFTStr += "CHAT ";
		    break;
		    case eFT_PRIVATE: mFTStr += "PRIVATE ";
		    break;
		    case eFT_SEARCH: mFTStr += "SEARCH ";
		    break;
		    case eFT_MYINFO: mFTStr += "MYINFO ";
		    break;
		    default: mFTStr += "UNKNOWN ";
		    break;
		}
	    }
	    return mFTStr;
	}
};

class cFloodCfg : public cConfigBase
{
public:
	cFloodCfg(cServerDC *);
	int mMaxConnPerIP;
	int mMaxUsersPerIP;
	int mBanTimeOnFlood;
	cServerDC *mS;
	virtual int Load();
	virtual int Save();
};

class cFloodprotect
{
public:
	cFloodprotect(cServerDC *);
	~cFloodprotect();
	bool CleanUp(int secs);
	bool CheckFlood(cConnDC *, tFloodType);
	bool AddConn(cConnDC *, short diff = 1);
	int KickAll(cConnDC *);

	cServerDC * mS;

	typedef tcHashListMap<sUserInfo *> tUserInfo;
	typedef tHashArray<short int> tConnCounter;
	typedef tUserInfo::iterator tUIIt;
	tUserInfo mUserInfo;
	tConnCounter mConnCounter;
	cFloodCfg mCfg;
};

};
};

#endif
