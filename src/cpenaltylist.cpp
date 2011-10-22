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
#include "cpenaltylist.h"
#include "cquery.h"
#include "i18n.h"
#include <sys/time.h>

namespace nVerliHub {
	using namespace nMySQL;
	namespace nTables {

cPenaltyList::cPenaltyList(cMySQL &mysql) : cConfMySQL(mysql), mCache(mysql, "temp_rights", "nick")
{
	mMySQLTable.mName = "temp_rights";
	AddCol("nick", "varchar(30)", "", false, mModel.mNick);
	AddPrimaryKey("nick");
	AddCol("since", "int(11)", "", true, mModel.mSince);
	AddCol("st_chat", "int(11)", "1", true, mModel.mStartChat);
	AddCol("st_search", "int(11)", "1", true, mModel.mStartSearch);
	AddCol("st_ctm", "int(11)", "1", true, mModel.mStartCTM);
	AddCol("st_pm", "int(11)", "1", true, mModel.mStartPM);
	AddCol("st_kick", "int(11)", "1", true,mModel.mStopKick);
	AddCol("st_share0", "int(11)", "1", true, mModel.mStopShare0);
	AddCol("st_reg", "int(11)", "1", true, mModel.mStopReg);
	AddCol("st_opchat", "int(11)", "1", true, mModel.mStopOpchat);
	mMySQLTable.mExtra = "PRIMARY KEY(nick), ";
	mMySQLTable.mExtra = "INDEX creation_index(since)";
	SetBaseTo(&mModel);
}


cPenaltyList::~cPenaltyList()
{
}

ostream &operator << (ostream &os, const cPenaltyList::sPenalty &penalty)
{
	cTime Now = cTime().Sec();

	if (penalty.mStartChat > Now)
		os << autosprintf(_("Setting main chat right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStartChat - Now).AsPeriod().AsString().c_str());

	if (penalty.mStartSearch > Now)
		os << autosprintf(_("Setting search right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStartSearch - Now).AsPeriod().AsString().c_str());

	if (penalty.mStartCTM > Now)
		os << autosprintf(_("Setting download right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStartCTM - Now).AsPeriod().AsString().c_str());

	if (penalty.mStartPM > Now)
		os << autosprintf(_("Setting private chat right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStartPM - Now).AsPeriod().AsString().c_str());

	if (penalty.mStopKick > Now)
		os << autosprintf(_("Setting kick right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStopKick - Now).AsPeriod().AsString().c_str());

	if (penalty.mStopShare0 > Now)
		os << autosprintf(_("Setting hidden share right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStopShare0 - Now).AsPeriod().AsString().c_str());

	if (penalty.mStopReg > Now)
		os << autosprintf(_("Setting registering right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStopReg - Now).AsPeriod().AsString().c_str());

	if (penalty.mStopOpchat > Now)
		os << autosprintf(_("Setting operator chat right for %s to: %s"), penalty.mNick.c_str(), cTime(penalty.mStopOpchat - Now).AsPeriod().AsString().c_str());

	return os;
}

void cPenaltyList::Cleanup()
{
	time_t Now = cTime().Sec();
	cQuery query(mMySQL);
	query.OStream() << "DELETE FROM " << mMySQLTable.mName << " WHERE since <" << Now - 3600*24*7 <<
		" AND st_kick != 0 AND st_share0 != 0 AND st_opchat != 0 AND st_reg != 0";
	query.Query();
	query.Clear();
}

bool cPenaltyList::LoadTo(sPenalty &pen, const string &Nick)
{
	if(mCache.IsLoaded() && !mCache.Find(Nick)) return false;
	SetBaseTo(&pen);
	pen.mNick = Nick;
	return LoadPK();
}

bool cPenaltyList::AddPenalty(sPenalty &penal)
{
	SetBaseTo(&mModel);
	mModel.mNick = penal.mNick;
	bool keep = false;
	if (LoadPK())
	{
		if(penal.mStartChat > mModel.mStartChat) mModel.mStartChat = penal.mStartChat;
		if(penal.mStartCTM > mModel.mStartCTM) mModel.mStartCTM = penal.mStartCTM;
		if(penal.mStartPM > mModel.mStartPM) mModel.mStartPM = penal.mStartPM;
		if(penal.mStartSearch > mModel.mStartSearch) mModel.mStartSearch = penal.mStartSearch;
		if(penal.mStopKick > mModel.mStopKick) mModel.mStopKick = penal.mStopKick;
		if(penal.mStopShare0 > mModel.mStopShare0) mModel.mStopShare0 = penal.mStopShare0;
		if(penal.mStopReg > mModel.mStopReg) mModel.mStopReg = penal.mStopReg;
		if(penal.mStopOpchat > mModel.mStopOpchat) mModel.mStopOpchat = penal.mStopOpchat;
		keep = mModel.ToKeepIt();
	}
	else
	{
		SetBaseTo(&penal);
		keep = penal.ToKeepIt();
		if (keep)
		{
			mCache.Add(penal.mNick);
		}
	}

	DeletePK();

	if( keep )
		return SavePK(false);
	else
		return false;
}

bool cPenaltyList::RemPenalty(sPenalty &penal)
{
	SetBaseTo(&mModel);
	mModel.mNick = penal.mNick;
	time_t Now = cTime().Sec();
	if(LoadPK()) {
		if(penal.mStartChat < Now) mModel.mStartChat = Now;
		if(penal.mStartCTM < Now) mModel.mStartCTM = Now;
		if(penal.mStartPM < Now) mModel.mStartPM = Now;
		if(penal.mStartSearch < Now) mModel.mStartSearch = Now;
		if(penal.mStopKick < Now) mModel.mStopKick = Now;
		if(penal.mStopShare0 < Now) mModel.mStopShare0 = Now;
		if(penal.mStopReg < Now) mModel.mStopReg = Now;
		if(penal.mStopOpchat < Now) mModel.mStopOpchat = Now;
	}
	if(mModel.ToKeepIt())
		return SavePK();
	else
		DeletePK();
	return true;
}

	}; // namespace nTables
}; // namespace nVerliHub
