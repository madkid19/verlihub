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
#include "cvhplugin.h"
#include "cuser.h"
#include "cserverdc.h"
#include "i18n.h"

namespace nDirectConnect {

namespace nPlugin {

cVHPlugin::cVHPlugin()
{
	mServer = 0;
	mUserDataTable = NULL;
}


cVHPlugin::~cVHPlugin()
{
	cUserCollection::iterator it;
	cPluginRobot * robot;
	for(it = mRobots.begin(); it != mRobots.end();) {
		robot = (cPluginRobot *) *it;
		++it;
		DelRobot(robot);
	}
	if (mUserDataTable) {
		delete mUserDataTable;
		mUserDataTable = NULL;
	}
}

bool cVHPlugin::AddRobot(cUserRobot *robot)
{
	if (!mServer->AddRobot(robot)) return false;
	if (!mRobots.Add((cUser*)robot)) {
		mServer->DelRobot(robot);
		return false;
	}
	return true;
}

cPluginRobot * cVHPlugin::NewRobot(const string &Nick, int uclass)
{
	cPluginRobot *robot = new cPluginRobot(Nick, this, mServer);
	//set class before adding to list, otherwise user can't be op
	robot->mClass = tUserCl(uclass);
	if (AddRobot(robot))
			return robot;
	else
	{
		delete robot;
		return NULL;
	}
}

bool cVHPlugin::DelRobot(cUserRobot *robot)
{
	bool result = mRobots.Remove(robot);
	mServer->DelRobot(robot);
	delete robot;
	return result;
}

bool cVHPlugin::AddScript(const string &filename, ostream &os)
{
	os << autosprintf(_("Plugin %s %s cannot load extra script."), mName.c_str(), mVersion.c_str());
	return false;
}

bool cVHPlugin::LoadScript(const string &filename, ostream &os)
{
	os << autosprintf(_("Plugin %s %s cannot load script '%s'."), mName.c_str(), mVersion.c_str(), filename.c_str());
	return false;
}

cPluginUserData *cVHPlugin::GetPluginUserData( cUser * User )
{
	if (mUserDataTable) {
		tHashArray<cPluginUserData*>::tHashType Hash = (tHashArray<cPluginUserData*>::tHashType) User; 
		return mUserDataTable->GetByHash(Hash);
	} else return NULL;
}

cPluginUserData *cVHPlugin::SetPluginUserData( cUser *User, cPluginUserData *NewData )
{
	if (!mUserDataTable && User && NewData) {
		mUserDataTable = new tHashArray<cPluginUserData*>();
	}
	tHashArray<cPluginUserData*>::tHashType Hash = (tHashArray<cPluginUserData*>::tHashType) User; 
	cPluginUserData *OldData = mUserDataTable->GetByHash(Hash);
	mUserDataTable->SetByHash(Hash, NewData);
	return OldData;
}

};

};
