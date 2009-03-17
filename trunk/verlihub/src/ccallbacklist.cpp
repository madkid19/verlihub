/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ccallbacklist.h"
#include "cpluginbase.h"
#include "cpluginmanager.h"

namespace nPlugin {

cCallBackList::cCallBackList(cPluginManager *mgr, string id) :
	mMgr(mgr), mCallOne(mMgr,this), mName(id)
{
	if(mMgr) mMgr->SetCallBack(id, this);
}

const string &cCallBackList::Name() const
{
	return mName;
}

cCallBackList::~cCallBackList(){}

void cCallBackList::ufCallOne::operator()(cPluginBase *pi)
{
	if( mCall ) mCall = mCBL->CallOne(pi);
	if (!pi->IsAlive()) mMgr->UnloadPlugin(pi->Name());
}

};


/*!
    \fn nPlugin::cCallBackList::Register(cPluginBase *pi)
 */
bool nPlugin::cCallBackList::Register(cPluginBase *pi)
{
	if(!pi) return false;
	tPICont::iterator i = find(mPlugins.begin(), mPlugins.end(),pi);
	if(i != mPlugins.end()) return false;
	mPlugins.push_back(pi);
	return true;
}


/*!
    \fn nPlugin::cCallBackList::Unregister(cPluginBase *pi)
 */
bool nPlugin::cCallBackList::Unregister(cPluginBase *pi)
{
	if(!pi) return false;
	tPICont::iterator i = find(mPlugins.begin(), mPlugins.end(),pi);
	if(i == mPlugins.end()) return false;
	mPlugins.erase(i);
	return true;
}

bool nPlugin::cCallBackList::CallAll()
{
	mCallOne.mCall = true;
	return for_each( mPlugins.begin() , mPlugins.end(), mCallOne ).mCall;
}

void nPlugin::cCallBackList::ListRegs(ostream &os, const char *indent)
{
	tPICont::iterator i;
	for (i = mPlugins.begin(); i != mPlugins.end(); ++i)
	{
		os << indent << (*i)->Name() << "\r\n";
	}
}
