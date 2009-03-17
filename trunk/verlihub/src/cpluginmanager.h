/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NPLUGINCPLUGINMANAGER_H
#define NPLUGINCPLUGINMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include "tchashlistmap.h"

using std::vector;
using std::string;
using std::ostream;

using namespace nUtils;

namespace nPlugin {

class cPluginLoader;
class cCallBackList;
class cPluginBase;
/**
the plugin manager, let's you to load, and unload plugins

@author Daniel Muller
*/
class cPluginManager : public cObj
{
public:
	cPluginManager(const string &dir);
	~cPluginManager();
	bool LoadAll();
	bool LoadPlugin(const string &);
	bool UnloadPlugin(const string &name);
	bool ReloadPlugin(const string &name);
	bool SetCallBack(string id, cCallBackList*);
	bool RegisterCallBack(string id, cPluginBase *pi);
	bool UnregisterCallBack(string id, cPluginBase *pi);
	void List(ostream &os);
	void ListAll(ostream &os);
	virtual void OnPluginLoad(cPluginBase *) = 0;
	typedef tcHashListMap<cPluginLoader*> tPlugins;
	string &GetError(){ return mLastLoadError;}
	cPluginBase * GetPlugin(const string &Name);
	cPluginBase * GetPluginByLib(const string &lib);
protected:
	string mPluginDir;
	typedef tcHashListMap<cCallBackList*> tCBList;
	tPlugins mPlugins;
	tCBList mCallBacks;
	string mLastLoadError;
};

};

#endif
