/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cinfoserver.h"
#include "cserverdc.h"
#include "curr_date_time.h"
#if ! defined _WIN32
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif


namespace nDirectConnect {
namespace nInfo {

cInfoServer::cInfoServer()
{
	mServer = NULL;
}

string cInfoServer::converByte(__int64 byte, bool UnitType)
{
	static const char *byteUnit[] = {"B", "KB", "MB", "GB", "TB", "PB", "", "", ""};
	static const char *byteSecUnit[] = {"B/s", "KB/s", "MB/s", "GB/s", "TB/s", "PB/s", "", "", ""};
	//string result;
	int unit;
	
	double long lByte = byte;
	
	if(lByte < 1024) {
		unit = 0;
	} else {
		for(unit = 0; lByte > 1024; unit++) {
			lByte /= 1024;	
		}
	}
	
	ostringstream os (ostringstream::out);
	os.precision(2);
	os << fixed << lByte << " ";
	if(UnitType)  os << byteSecUnit[unit];
	else os << byteUnit[unit];
	return os.str();
}

void cInfoServer::SystemInfo(ostream &os)
{
	#if ! defined _WIN32
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
	os << "\r\n[::] System uptime: "  << uptime.AsPeriod() << "\r\n";
	os << "[::] Load averages: " <<  std::fixed << std::setprecision(2) << serverInfo.loads[0]/65536.0 << " " << serverInfo.loads[1]/65536.0 << " " << serverInfo.loads[2]/65536.0 << "\r\n";
	os << "[::] Used RAM: "  << converByte((long long int) serverInfo.totalram,false) << "\r\n";
	os << "[::] Free RAM: "  << converByte((long long int) serverInfo.freeram,false) << "\r\n";
	os << "[::] Shared RAM: "  << converByte((long long int) serverInfo.sharedram,false) << "\r\n";
	os << "[::] Memory in buffers: "  << converByte((long long int) serverInfo.bufferram,false) << "\r\n";
	os << "[::] Total swap: "  << converByte((long long int) serverInfo.totalswap,false) << "\r\n";
	os << "[::] Free swap: "  << converByte((long long int) serverInfo.freeswap,false) << "\r\n";
	os << "[::] Number of processes: "  << serverInfo.procs << "\r\n";



        struct rusage resourceUsage; 

        getrusage(RUSAGE_SELF, &resourceUsage);


	#else 
	os << "No info available\r\n";
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
	os << "\r\n""[::] Version date: "  __CURR_DATE_TIME__ "\r\n";
	theTime = mServer->mTime;
	theTime -= mServer->mStartTime;
	os << "[::] Uptime count: " << theTime.AsPeriod() << "\r\n";
	os << "[::] Server frequency: " << mServer->mFrequency.GetMean(mServer->mTime) << "\r\n";
	for(it = begin(); it != end(); ++it)
		os << (*it)->mName << " : " << *(*it) << "\r\n";
	os << "[::] Current online users: " << mServer->mUserCountTot << "\r\n";;
	os << "[::] Current share total: " << converByte(mServer->mTotalShare, false) << "\r\n";
	os << "[::] User list count: " << mServer->mUserList.size() << "\r\n";
	os << "[::] Hello user count: " << mServer->mHelloUsers.size() << "\r\n";
	os << "[::] In Progress users: " << mServer->mInProgresUsers.size() << "\r\n";
	os << "[::] Active user count: " << mServer->mActiveUsers.size() << "\r\n";
	os << "[::] Op user count: " << mServer->mOpchatList.size() << "\r\n";
	os << "[::] Bot user count: " << mServer->mRobotList.size() << "\r\n";
	double total = 0, curr;
	int i = 0;
	for( i =0; i <= USER_ZONES; i++)
	{
		curr = mServer->mUploadZone[i].GetMean(mServer->mTime);
		os << "[::] Upload zone " << i << ": " << converByte(curr,true) << "\r\n";
		total += curr;
	}
	os << "[::] Current upload total for all zones: " << converByte(total,true) << "\r\n";
 	for( i =0; i < 4; i++) {
		os << "[::] Zone (" << i << ") ";
		if(i > 0) os << mServer->mC.cc_zone[i-1];
		else os << "all";
		os << " users: " << mServer->mUserCount[i] << " / " <<mServer->mC.max_users[i] << "\r\n";
	}
	for (i=4; i <= USER_ZONES; i++)
		os << "[::] Zone (" << i << ") IP-Range : " << mServer->mUserCount[i] << " / " <<mServer->mC.max_users[i] << "\r\n";
}

cInfoServer::~cInfoServer(){}
};
};

