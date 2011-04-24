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
#include "cpistats.h"

namespace nVerliHub {
	using namespace nSocket;
	using namespace nEnums;
	namespace nStatsPlugin {
cpiStats::cpiStats() :
	mStats(NULL),
	mStatsTimer(300.0,0.0,cTime().Sec()),
	mFreqSearchA(cTime(), 300.0, 10),
	mFreqSearchP(cTime(), 300.0, 10)
{
	mName = "Stats";
	mVersion = STATS_VERSION;
}

void cpiStats::OnLoad(cServerDC *server)
{
	cVHPlugin::OnLoad(server);
	mServer = server;
	mStats = new cStats(server);
	mStats->CreateTable();
}

bool cpiStats::RegisterAll()
{
	RegisterCallBack("VH_OnUserCommand");
	RegisterCallBack("VH_OnTimer");
	RegisterCallBack("VH_OnParsedMsgSearch");
	return true;
}

bool cpiStats::OnTimer()
{
	if(mStatsTimer.Check(this->mServer->mTime , 1) == 0)  {
		this->mStats->mTime = this->mServer->mTime.Sec();
		int i = 0;
		mStats->mUploadTotalBps = 0;
		double curr;
		for(i = 0; i <= USER_ZONES; i++) {
			//cout << "Zone " << i << cTime().AsDate() << endl;
			//mServer->mUploadZone[i].Dump();
			curr = mServer->mUploadZone[i].GetMean(mServer->mTime);
			//mServer->mUploadZone[i].Dump();
			mStats->mUploadInZonesBps[i] = curr;
			mStats->mUploadTotalBps += curr;
		}
		// calculate total share in GB
		mStats->mShareTotalGB = mServer->mTotalShare /(1024*1024*1024);
		// calculate the uptime
		cTime theUpTime(mServer->mTime);
		theUpTime -= mServer->mStartTime;
		mStats->mUptimeSec = theUpTime.Sec();
		// calculate search frequencys
		mStats->mFreqSearchA = this->mFreqSearchA.GetMean(mServer->mTime);
		mStats->mFreqSearchP = this->mFreqSearchP.GetMean(mServer->mTime);
		// save and clean
		this->mStats->Save();
		this->mStats->CleanUp();
	}
	return true;
}

bool cpiStats::OnParsedMsgSearch(cConnDC *, cMessageDC *msg)
{
	switch(msg->mType) {
		case eDC_MSEARCH:
		case eDC_SEARCH:
			mFreqSearchA.Insert(cTime());
			break;
		case eDC_MSEARCH_PAS:
		case eDC_SEARCH_PAS:
			mFreqSearchP.Insert(cTime());
			break;
		default:break;
	}
	return true;
}

cpiStats::~cpiStats()
{
	if (mStats)
		delete mStats;
}
	}; // namespace nStatsPlugin
}; // namespace nVerliHub
REGISTER_PLUGIN(nVerliHub::nStatsPlugin::cpiStats);

