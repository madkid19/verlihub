/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cvhplugin.h"
#include "cuser.h"
#include "cserverdc.h"

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
	os << "Plugin " << mName << " " << mVersion << " cannot load extra script. ";
	return false;
}

bool cVHPlugin::LoadScript(const string &filename, ostream &os)
{
	os << "Plugin " << mName << " " << mVersion << " cannot load script '" << filename << ". ";
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
