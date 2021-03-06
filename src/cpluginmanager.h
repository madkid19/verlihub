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
#ifndef NPLUGINCPLUGINMANAGER_H
#define NPLUGINCPLUGINMANAGER_H
#include <vector>
#include <string>
#include <iostream>
#include "tchashlistmap.h"

using std::vector;
using std::string;
using std::ostream;

namespace nVerliHub {
	namespace nPlugin {
		using namespace nUtils;
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
}; // namespace nPlugin

}; // namespace nVerliHub

#endif
