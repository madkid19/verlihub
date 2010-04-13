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
#include "ccallbacklist.h"
#include "cpluginbase.h"
#include "cpluginmanager.h"
#ifdef WIN32
#pragma warning( disable : 4355)
#endif

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
