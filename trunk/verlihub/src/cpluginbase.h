/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NPLUGINCPLUGINBASE_H
#define NPLUGINCPLUGINBASE_H

#include <string>
#include "cobj.h"

#ifndef REGISTER_PLUGIN
#define REGISTER_PLUGIN(__classname) \
	extern "C" {\
		cPluginBase * get_plugin(void){ return new (__classname); }\
		void del_plugin(cPluginBase *plugin)\
		{if(plugin) delete plugin;}\
	}
#endif

using std::string;

namespace nPlugin {

class cPluginManager;
/**
the plugin base class suitable for any application

@author Daniel Muller
*/
class cPluginBase : public cObj
{
private:
	bool mIsAlive;
public:
	cPluginBase();
	virtual ~cPluginBase();
	const string &Name(){return mName;}
	const string &Version(){return mVersion;}
	void Suicide(){mIsAlive = false;}
	bool IsAlive(){return mIsAlive;}
	void SetMgr( cPluginManager *mgr){ mManager = mgr; };
	bool RegisterCallBack(string id);
	bool UnRegisterCallBack(string id);

	virtual int StrLog(ostream & ostr, int level);
	virtual bool RegisterAll() = 0;
protected:
	string mName;
	string mVersion;
	cPluginManager * mManager;
};

};

#endif
