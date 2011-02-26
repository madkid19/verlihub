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

#ifndef CPLUG_H
#define CPLUG_H

#include <string>
#include <ctime.h>
#include <tlistplugin.h>

using namespace std;

namespace nDirectConnect {
	namespace nPlugin {
		class cVHPluginMgr;
	};
};
using namespace nDirectConnect::nPlugin;
class cpiPlug;
class cPlugs;

enum { ePLUG_AUTOLOAD, ePLUG_LOAD, ePLUG_AUTORELOAD, ePLUG_RELOAD, ePLUG_AUTOUNLOAD, ePLUG_UNLOAD };

class cPlug
{
public:
	// -- stored data
	string mNick; // plugin's nick, easy to remember
	string mPath; // the fiilename to load
	string mDesc; // public description
	string mDest; // destination plugin
	bool mLoadOnStartup;
	bool mReloadNext; // reload if it's loaded
	bool mUnloadNext; // unload if it's loaded
	string mLastError;
	unsigned mLoadTime;

	// -- memory data
	string mLastVersion;
	string mLastName;
	cPlugs *mOwner;
	time_t mMakeTime;

	// -- methods
	cVHPlugin *IsLoaded() const;
	bool Plugin();
	bool Plugout();
	bool Replug();
	bool CheckMakeTime();
	void SaveMe();
	bool IsScript() const;
	cPlug *FindDestPlugin() const;
	cVHPlugin *GetDestPlugin() const;

	// -- required methods
	cPlug();
	virtual void OnLoad();
	friend ostream& operator << (ostream &, const cPlug &plug);

};

typedef class tList4Plugin<cPlug, cpiPlug> tPlugListBase;

class cPlugs : public tPlugListBase
{
public:

	// -- usable methods
	cPlug *FindPlug(const string &nick);
	void PluginAll(int method);
	time_t GetFileTime(const string &filename);

	// -- required methods
	cPlugs(cVHPlugin *pi);
	virtual void AddFields();
	virtual bool CompareDataKey(const cPlug &D1, const cPlug &D2);

	// -- optional overrides
	virtual void OnLoadData(cPlug &plug);

	// -- data
	cVHPluginMgr *mPM;
	time_t mVHTime;

};

#endif//CPLUG_H
