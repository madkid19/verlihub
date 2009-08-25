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
#include "cinfoserver.h"
#include "stringutils.h"
#include "cserverdc.h"
#include "curr_date_time.h"
#if ! defined _WIN32 && ! defined HAVE_FREEBSD
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif
using namespace nStringUtils;
namespace nDirectConnect {
namespace nInfo {

cInfoServer::cInfoServer()
{
	mServer = NULL;
}

void cInfoServer::SystemInfo(ostream &os)
{
	#if ! defined _WIN32 && ! defined HAVE_FREEBSD
	struct sysinfo serverInfo;
	if(sysinfo(&serverInfo)) {
		os << "Cannot retrive system information";
		return;
	}
	cTime uptime(serverInfo.uptime);
	utsname osname;
	if(uname(&osname) == 0) { 
		os << "\r\n[::] OS: " << osname.sysname << " " << osname.release << " (" << osname.machine << ") ";
	}
	os << "\r\n[::] System uptime: "  << uptime.AsPeriod() << endl;
	os << "[::] Load averages: " <<  std::fixed << std::setprecision(2) << serverInfo.loads[0]/65536.0 << " " << serverInfo.loads[1]/65536.0 << " " << serverInfo.loads[2]/65536.0 << endl;
	os << "[::] Used RAM: "  << convertByte((long long int) serverInfo.totalram,false) << endl;
	os << "[::] Free RAM: "  << convertByte((long long int) serverInfo.freeram,false) << endl;
	os << "[::] Shared RAM: "  << convertByte((long long int) serverInfo.sharedram,false) << endl;
	os << "[::] Memory in buffers: "  << convertByte((long long int) serverInfo.bufferram,false) << endl;
	os << "[::] Total swap: "  << convertByte((long long int) serverInfo.totalswap,false) << endl;
	os << "[::] Free swap: "  << convertByte((long long int) serverInfo.freeswap,false) << endl;
	os << "[::] Number of processes: "  << serverInfo.procs << endl;



        struct rusage resourceUsage; 

        getrusage(RUSAGE_SELF, &resourceUsage);


	#else 
	os << "No info available" << endl;
	#endif
}

void cInfoServer::SetServer(cServerDC *Server)
{
	mServer = Server;
	Add("[::] Hub name", mServer->mC.hub_name);
	Add("[::] Hub Owner", mServer->mC.hub_owner);
	Add("[::] Hub Category", mServer->mC.hub_category);
	Add("[::] Total user limit",mServer->mC.max_users_total);
}

void cInfoServer::Output(ostream &os, int Class)
{
	iterator it;
	cTime theTime;
	os << "\r\n""[::] Version date: "  __CURR_DATE_TIME__ << endl;
	theTime = mServer->mTime;
	theTime -= mServer->mStartTime;
	os << "[::] Uptime: " << theTime.AsPeriod() << endl;
	os << "[::] Server frequency: " << mServer->mFrequency.GetMean(mServer->mTime)  << " (";
	if (mServer->mSysLoad >= eSL_RECOVERY) os << "Recovery mode";
	if (mServer->mSysLoad >= eSL_CAPACITY) os << "Near capacity";
	if (mServer->mSysLoad >= eSL_PROGRESSIVE) os << "Progressive mode";
	if (mServer->mSysLoad >= eSL_NORMAL) os << "Normal mode";
	os << ")" << endl;
	for(it = begin(); it != end(); ++it)
		os << (*it)->mName << " : " << *(*it) << endl;
	os << "[::] Current online users: " << mServer->mUserCountTot << endl;
	os << "[::] Current share total: " << convertByte(mServer->mTotalShare, false) << endl;
	os << "[::] User list count: " << mServer->mUserList.size() << endl;
	os << "[::] Hello user count: " << mServer->mHelloUsers.size() << endl;
	os << "[::] In Progress users: " << mServer->mInProgresUsers.size() << endl;
	os << "[::] Active user count: " << mServer->mActiveUsers.size() << endl;
	os << "[::] Op user count: " << mServer->mOpchatList.size() << endl;
	os << "[::] Bot user count: " << mServer->mRobotList.size() << endl;
	double total = 0, curr;
	int i = 0;
	for( i =0; i <= USER_ZONES; i++)
	{
		curr = mServer->mUploadZone[i].GetMean(mServer->mTime);
		os << "[::] Upload zone " << i << ": " << convertByte(curr,true) << endl;
		total += curr;
	}
	os << "[::] Current upload total for all zones: " << convertByte(total,true) << endl;
 	for( i =0; i < 4; i++) {
		os << "[::] Zone (" << i << ") ";
		if(i > 0) os << mServer->mC.cc_zone[i-1];
		else os << "all";
		os << " users: " << mServer->mUserCount[i] << " / " <<mServer->mC.max_users[i] << endl;
	}
	for (i=4; i <= USER_ZONES; i++)
		os << "[::] Zone (" << i << ") IP-Range : " << mServer->mUserCount[i] << " / " <<mServer->mC.max_users[i] << endl;
}

cInfoServer::~cInfoServer(){}
};
};

