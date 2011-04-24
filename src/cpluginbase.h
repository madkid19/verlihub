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
#ifndef NPLUGINCPLUGINBASE_H
#define NPLUGINCPLUGINBASE_H
#include <string>
#include "cobj.h"
#ifndef REGISTER_PLUGIN
#define REGISTER_PLUGIN(__classname) \
	extern "C" {\
		nVerliHub::nPlugin::cPluginBase * get_plugin(void){ return new (__classname); }\
		void del_plugin(nVerliHub::nPlugin::cPluginBase *plugin)\
		{if(plugin) delete plugin;}\
	}
#endif

using std::string;
namespace nVerliHub {
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

	}; // namespace nPlugin
}; // namespace nVerliHub

#endif
