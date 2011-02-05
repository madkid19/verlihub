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
#if ! defined _WIN32 && ! defined HAVE_FREEBSD && ! defined HAVE_APPLE
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include "i18n.h"
using namespace nStringUtils;
namespace nDirectConnect {
namespace nInfo {

cInfoServer::cInfoServer()
{
	mServer = NULL;
}

void cInfoServer::SystemInfo(ostream &os)
{
	#if ! defined _WIN32 && ! defined HAVE_FREEBSD && ! defined HAVE_APPLE
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
	os << "\r\n[::] " << autosprintf(_("System uptime: %s"), uptime.AsPeriod().AsString().c_str()) << endl;
	os << "[::] " << autosprintf(_("Load averages: %.2f %.2f %.2f"), serverInfo.loads[0]/65536.0, serverInfo.loads[1]/65536.0, serverInfo.loads[2]/65536.0) << endl;
	os << "[::] " << autosprintf(_("Used RAM: %s"), convertByte((long long int) serverInfo.totalram,false).c_str()) << endl;
	os << "[::] " << autosprintf(_("Free RAM: %s"), convertByte((long long int) serverInfo.freeram,false).c_str()) << endl;
	os << "[::] " << autosprintf(_("Shared RAM: %s"), convertByte((long long int) serverInfo.sharedram,false).c_str()) << endl;
	os << "[::] " << autosprintf(_("Memory in buffers: %s"), convertByte((long long int) serverInfo.bufferram,false).c_str()) << endl;
	os << "[::] " << autosprintf(_("Total swap: %s"), convertByte((long long int) serverInfo.totalswap,false).c_str()) << endl;
	os << "[::] " << autosprintf(_("Free swap: %s"), convertByte((long long int) serverInfo.freeswap,false).c_str()) << endl;
	os << "[::] " << autosprintf(_("Number of processes: %d"), serverInfo.procs) << endl;



        struct rusage resourceUsage; 

        getrusage(RUSAGE_SELF, &resourceUsage);

	#else 
	os << _("No info available") << endl;
	#endif
}

void cInfoServer::SetServer(cServerDC *Server)
{
	mServer = Server;
	ostringstream description;
	description << "[::] " << _("Hub name");
	Add(description.str(), mServer->mC.hub_name);
	description.str("");
	description << "[::] " << _("Hub owner");
	Add(description.str(), mServer->mC.hub_owner);
	description.str("");
	description << "[::] " << _("Hub category");
	Add(description.str(), mServer->mC.hub_category);
	description.str("");
	description << "[::] " << _("Total user limit");
	Add(description.str(),mServer->mC.max_users_total);
}

void cInfoServer::Output(ostream &os, int Class)
{
	iterator it;
	cTime theTime;
	os << "\r\n""[::] " << autosprintf(_("Version date: %s"), __CURR_DATE_TIME__) << endl;
	theTime = mServer->mTime;
	theTime -= mServer->mStartTime;
	os << "[::] " << autosprintf(_("Uptime: %s"), theTime.AsPeriod().AsString().c_str()) << endl;
	string loadType;
	if(mServer->mSysLoad >= eSL_RECOVERY)
		loadType = _("Recovery mode");
	if(mServer->mSysLoad >= eSL_CAPACITY)
		loadType = _("Near capacity");
	if(mServer->mSysLoad >= eSL_PROGRESSIVE)
		loadType = _("Progressive mode");
	if(mServer->mSysLoad >= eSL_NORMAL)
		loadType = _("Normal mode");
	os << "[::] " << autosprintf(_("Server frequency: %.4f (%s)"), mServer->mFrequency.GetMean(mServer->mTime), loadType.c_str()) << endl;
	
	for(it = begin(); it != end(); ++it)
		os << (*it)->mName << " : " << *(*it) << endl;
	os << "[::] " << autosprintf(_("Current online users: %d") ,mServer->mUserCountTot) << endl;
	os << "[::] " << autosprintf(_("Current share total: %s"), convertByte(mServer->mTotalShare, false).c_str()) << endl;
	os << "[::] " << autosprintf(_("User list count: %d"), mServer->mUserList.size()) << endl;
	os << "[::] " << autosprintf(_("Hello user count: %d"), mServer->mHelloUsers.size()) << endl;
	os << "[::] " << autosprintf(_("In Progress users: %d"), mServer->mInProgresUsers.size()) << endl;
	os << "[::] " << autosprintf(_("Active user count: %d"), mServer->mActiveUsers.size()) << endl;
	os << "[::] " << autosprintf(_("Passive user count: %d"), mServer->mPassiveUsers.size()) << endl;
	os << "[::] " << autosprintf(_("Op user count: %d"), mServer->mOpchatList.size()) << endl;
	os << "[::] " << autosprintf(_("Bot user count: %d"), mServer->mRobotList.size()) << endl;
	double total = 0, curr;
	int i = 0;
	for( i =0; i <= USER_ZONES; i++)
	{
		curr = mServer->mUploadZone[i].GetMean(mServer->mTime);
		os << "[::] " << autosprintf(_("Upload zone %d: %s"), i, convertByte(curr,true).c_str()) << endl;
		total += curr;
	}
	os << "[::] " << autosprintf(_("Current upload total for all zones: %s"), convertByte(total,true).c_str()) << endl;
 	for( i =0; i < 4; i++)
		os << "[::] " << autosprintf(_("Zone (%d) %s users: %d / %d"), i, (i > 0 ? mServer->mC.cc_zone[i-1].c_str() : _("all")), mServer->mUserCount[i], mServer->mC.max_users[i]) << endl;
	for (i=4; i <= USER_ZONES; i++)
		os << "[::] " << autosprintf(_("Zone (%d) IP-Range: %d / %d"), i, mServer->mUserCount[i], mServer->mC.max_users[i]) << endl;
}

cInfoServer::~cInfoServer(){}
};
};

