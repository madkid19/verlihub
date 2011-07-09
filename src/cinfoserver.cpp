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

#define PADDING 25

namespace nVerliHub {
	using namespace nUtils;
	using namespace nSocket;
cInfoServer::cInfoServer()
{
	mServer = NULL;
}

void cInfoServer::SystemInfo(ostream &os)
{
	#if ! defined _WIN32 && ! defined HAVE_FREEBSD && ! defined HAVE_APPLE
	struct sysinfo serverInfo;
	if(sysinfo(&serverInfo)) {
		os << _("Cannot retrive system information");
		return;
	}
	cTime uptime(serverInfo.uptime);
	utsname osname;
	if(uname(&osname) == 0) {
		os << "\r\n[*] " << setw(PADDING) << setiosflags(ios::left) << "OS" << osname.sysname << " " << osname.release << " (" << osname.machine << ") ";
	}
	os << "\r\n[*] " << setw(PADDING) << setiosflags(ios::left) << _("System uptime") << uptime.AsPeriod().AsString().c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Load averages") << autosprintf("%.2f %.2f %.2f", serverInfo.loads[0]/65536.0, serverInfo.loads[1]/65536.0, serverInfo.loads[2]/65536.0) << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Used RAM") << convertByte((long long int) serverInfo.totalram,false).c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Free RAM") << convertByte((long long int) serverInfo.freeram,false).c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Shared RAM") << convertByte((long long int) serverInfo.sharedram,false).c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Memory in buffers") << convertByte((long long int) serverInfo.bufferram,false).c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Frew swap") << convertByte((long long int) serverInfo.freeswap,false).c_str() << "/" << convertByte((long long int) serverInfo.totalswap,false).c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Number of processes") << serverInfo.procs << endl;

        struct rusage resourceUsage;

        getrusage(RUSAGE_SELF, &resourceUsage);

	#else
	os << _("No info available") << endl;
	#endif
}

void cInfoServer::SetServer(cServerDC *Server)
{
	mServer = Server;

}

void cInfoServer::Output(ostream &os, int Class)
{
	iterator it;
	cTime theTime;
	os << "\r\n[*] " << setw(PADDING) << setiosflags(ios::left) << _("Version date") << __CURR_DATE_TIME__ << endl;
	theTime = mServer->mTime;
	theTime -= mServer->mStartTime;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Uptime") << theTime.AsPeriod().AsString().c_str() << endl;
	string loadType;
	if(mServer->mSysLoad >= eSL_RECOVERY)
		loadType = _("Recovery mode");
	if(mServer->mSysLoad >= eSL_CAPACITY)
		loadType = _("Near capacity");
	if(mServer->mSysLoad >= eSL_PROGRESSIVE)
		loadType = _("Progressive mode");
	if(mServer->mSysLoad >= eSL_NORMAL)
		loadType = _("Normal mode");
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Server frequency") << mServer->mFrequency.GetMean(mServer->mTime) << " " << "(" << loadType.c_str() << ")" << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Hub name") << mServer->mC.hub_name << endl;

	string hubOwner;
		if(!mServer->mC.hub_owner.empty())
		hubOwner = mServer->mC.hub_owner;
	else
		hubOwner = "--";
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Hub owner") << hubOwner << endl;

	string hubCategory;
		if(!mServer->mC.hub_category.empty())
		hubCategory = mServer->mC.hub_category;
	else
		hubCategory = "--";
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Hub category") << hubCategory << endl;
	os << "    " << string(30,'=') << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Current online users") << mServer->mUserCountTot << "/" << mServer->mC.max_users_total << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Current share total") << convertByte(mServer->mTotalShare, false).c_str() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("User list count") << mServer->mUserList.Size() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Hello user count") << mServer->mHelloUsers.Size() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("In Progress users") << mServer->mInProgresUsers.Size() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Active user count") << mServer->mActiveUsers.Size() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("Passive user count") << mServer->mPassiveUsers.Size() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("OP user count") << mServer->mOpchatList.Size() << endl;
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) <<_("Bot user count") << mServer->mRobotList.Size() << endl;
	os << "    " << string(30,'=') << endl;
	double total = 0, curr;

	// Print zone from 1->6
	for(int i=1; i < 4; i++) {
		if(!mServer->mC.cc_zone[i-1].empty()) {
			string zone = mServer->mC.cc_zone[i-1];
			replace(zone.begin(), zone.end(), ':', ',');
			os << "[*] " << setw(PADDING) << setiosflags(ios::left) << autosprintf(_("Users in zone #%d"), i) << mServer->mUserCount[i] << "/" << mServer->mC.max_users[i];
			curr = mServer->mUploadZone[i].GetMean(mServer->mTime);
			total += curr;
			os << " (" << convertByte(curr,true).c_str() << ")" << " [" << zone.c_str() << "]" << endl;
		} else
			os << "[*] " << setw(PADDING) << setiosflags(ios::left) << autosprintf(_("Users in zone #%d"), i) << _("not set") << endl;
	}
	for (int i=4; i <= USER_ZONES; i++) {
		curr = mServer->mUploadZone[i].GetMean(mServer->mTime);
		total += curr;
		os << "[*] " << setw(PADDING) << setiosflags(ios::left) << autosprintf(_("Users in zone #%d"), i) << mServer->mUserCount[i] << "/" << mServer->mC.max_users[i];
		os << " (" << convertByte(curr,true).c_str() << ")" << " [IP-range]" << endl;
	}
	// Print zone 0
	os << "[*] " << setw(PADDING) << setiosflags(ios::left) << _("All other users") << mServer->mUserCount[0] << "/" << mServer->mC.max_users[0] << " (" << convertByte(total,true).c_str() << ")" << endl;
}

cInfoServer::~cInfoServer(){}
}; // namespace nVerliHub
