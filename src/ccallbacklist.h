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
#ifndef NPLUGINCCALLBACKLIST_H
#define NPLUGINCCALLBACKLIST_H
#include <list>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>

using namespace std;

/**
  * General usage plugin library
*/
namespace nPlugin {

class cPluginBase;
class cPluginManager;

/**
allows cPluginBase to be registered or unregistered for callback

@author Daniel Muller
*/

class cCallBackList
{
private:
	typedef list<cPluginBase*> tPICont;
	tPICont mPlugins;
	cPluginManager *mMgr;
public:
	/** calls one given subscribed callback function for plugin .. the CallOne
	\sa cCallBackList::CallOne(cPluginBase *pi)
	*/
	struct ufCallOne : public unary_function<void,tPICont::iterator> {
		cPluginManager *mMgr;
		cCallBackList *mCBL;
		bool mCall;
		ufCallOne(cPluginManager *mgr, cCallBackList *cbl) : mMgr(mgr), mCBL(cbl), mCall(true) {};
		void operator()(cPluginBase *pi);
	};
private:
	ufCallOne mCallOne;
	string mName;
public:
	cCallBackList(cPluginManager *mgr, string id);
	virtual ~cCallBackList();
	bool Register(cPluginBase *pi);
	bool Unregister(cPluginBase *pi);
	virtual void ListRegs(ostream &os, const char *indent);
	virtual bool CallAll();
	virtual const string &Name() const;

	// override this, it it returns false, callback should stop here
	virtual bool CallOne(cPluginBase *pi) = 0;
};

};

#endif
