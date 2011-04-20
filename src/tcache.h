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
#ifndef NCONFIGTCACHE_H
#define NCONFIGTCACHE_H
#include "thasharray.h"
#include "cconfmysql.h"
#include "ctime.h"

namespace nVerliHub {
	namespace nConfig {
		using namespace nUtils;
/**
allows to find faster whether user is NOT registered

@author Daniel Muller
*/

template <class IndexType>
class tCache : public cConfMySQL
{
public:
	tCache(nMySQL::cMySQL &mysql, const char* TableName, const char* IndexName, const char* DateName = NULL) :
		cConfMySQL(mysql), mDateName(DateName)
	{
		SetClassName("tCache");
		mMySQLTable.mName = TableName;
		cConfMySQL::Add(IndexName,mCurIdx);
		SetBaseTo(this);
		mIsLoaded = false;
	}

	~tCache(){ Clear(); }
	typedef tHashArray<void*> tHashTab;
	typedef tHashTab::tHashType tCacheHash;

	// remove all entrys from the cache
	void Clear()
 	{
		mHashTab.Clear();
		mIsLoaded = false;
	}

	// LoadAll has been called already?
	bool IsLoaded(){ return mIsLoaded; }

	// performs the db querya nd stored results
	int LoadAll()
	{
		SelectFields(mQuery.OStream());
		db_iterator it;
		for(it = db_begin(); it != db_end(); ++it) Add(mCurIdx);
		mQuery.Clear();

		if(Log(0)) LogStream() << mHashTab.size() << " items preloaded." << endl;
		mIsLoaded = (mHashTab.size() > 0);
		mLastUpdate.Get();
		return mHashTab.size();
	}

	// queries db with the time condition and adds the difference results
	int Update()
	{
		int n = 0;

		SelectFields(mQuery.OStream());
		if (mDateName) mQuery.OStream() << " WHERE " << mDateName << " > " << mLastUpdate.Sec();
		db_iterator it;
		for(it = db_begin(); it != db_end(); ++it)
		{
			if(!Find(mCurIdx)) Add(mCurIdx);
			n++;
		}
		if(n && Log(0)) LogStream() << mHashTab.size() << " items in cache," << n << " of it are just loaded" << endl;
		mQuery.Clear();
		mLastUpdate.Get();
		return n;
	}

 	tCacheHash CalcHash(string const &index)
 	{
 		return mHashTab.HashStringLower(index);
 	}

	void Add(IndexType const &index)
	{
		tCacheHash Hash = CalcHash(index);
		mHashTab.AddWithHash(this,Hash);
	}

	bool Find(IndexType const &index)
	{
		tCacheHash Hash = CalcHash(index);
		return mHashTab.ContainsHash(Hash);
	}

	void Sync(){ mLastSync.Get();}
	unsigned GetSync(){return mLastSync.Sec();}

protected:

	tHashTab mHashTab;
	bool mIsLoaded;
	cTime mLastUpdate;
	cTime mLastSync;
	const char *mDateName;
private:
	IndexType mCurIdx;
};

	}; // namespace nConfig
}; // namespace nVerliHub

#endif
