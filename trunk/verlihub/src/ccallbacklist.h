/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
