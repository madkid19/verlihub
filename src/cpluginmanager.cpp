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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <sys/types.h>
#include <iomanip>
#include <dirent.h>
#include "cpluginmanager.h"
#include "cpluginloader.h"
#include "ccallbacklist.h"
#include "stringutils.h"
#include "i18n.h"

namespace nVerliHub {
	using namespace nUtils;

	namespace nPlugin {

cPluginManager::cPluginManager(const string &path) :
	cObj("cPluginMgr"),
	mPluginDir(path)
{
	if (mPluginDir[mPluginDir.size()-1] != '/')
		mPluginDir.append("/");
}

cPluginManager::~cPluginManager(){}

bool cPluginManager::LoadAll()
{
	if(Log(0))
		LogStream() << "Open dir: " << mPluginDir << endl;
	DIR *dir = opendir(mPluginDir.c_str());
	if(dir == NULL) {
		if(Log(1))
			LogStream() << "Open dir error" << endl;
		return false;
	}
	struct dirent * ent = NULL;

	string filename;
	string pathname;

	while (NULL!= (ent=readdir(dir))) {
		filename = ent->d_name;
		if(Log(3))
			LogStream() << "filename: " << filename << endl;
		if((filename.size() > 3) && (0 == StrCompare(filename,filename.size()-3,3,".so"))) {
			pathname = mPluginDir+filename;
			LoadPlugin(pathname);
		}
	}
	closedir(dir);
	return true;
}

bool cPluginManager::LoadPlugin(const string &file)
{
#if ! defined _WIN32
	cPluginLoader *plugin;
	mLastLoadError = "";
	if(Log(3))
		LogStream() << "Attempt loading plugin: " << file << endl;
	plugin = new cPluginLoader(file);
	if(!plugin)
		return false;
	if(!plugin->Open() || !plugin->LoadSym() || !mPlugins.AddWithHash(plugin, mPlugins.Key2Hash(plugin->mPlugin->Name()))) {
		mLastLoadError = plugin->Error();
		delete plugin;
		return false;
	}

	try {
		plugin->mPlugin->SetMgr(this);
		plugin->mPlugin->RegisterAll();

		OnPluginLoad(plugin->mPlugin);
	} catch (...) {
		if(ErrLog(1))
			LogStream() << "Plugin " << file << " caused an exception" << endl;
	}
	if(Log(1))
		LogStream() << "Succes loading plugin: " << file << endl;
#endif
	return true;
}

bool cPluginManager::UnloadPlugin(const string &name)
{
	cPluginLoader *plugin = mPlugins.GetByHash(mPlugins.Key2Hash(name));
	if(!plugin || !mPlugins.RemoveByHash(mPlugins.Key2Hash(name))) {
		if(ErrLog(2))
			LogStream() << "Can't unload plugin name: '" << name << "'" << endl;
		return false;
	}

	tCBList::iterator it;
	for(it=mCallBacks.begin();it!=mCallBacks.end(); ++it)
		(*it)->Unregister(plugin->mPlugin);
	delete plugin;
	return true;
}

bool cPluginManager::ReloadPlugin(const string &name)
{
	cPluginLoader *plugin = mPlugins.GetByHash(mPlugins.Key2Hash(name));
	if (plugin) {
		string filename = plugin->GetFilename();
		if(!UnloadPlugin(name))
			return false;
		if(!LoadPlugin(filename))
			return false;
		return true;
	}
	return false;
}

bool cPluginManager::SetCallBack(string  id, cCallBackList *cb)
{
	if(!cb)
		return false;
	if(id.size() == 0)
		return false;
	return mCallBacks.AddWithHash(cb,mCallBacks.Key2Hash(id));
}

bool cPluginManager::RegisterCallBack(string id, cPluginBase *pi)
{
	cCallBackList *cbl = mCallBacks.GetByHash(mCallBacks.Key2Hash(id));
	if(!cbl)
		return false;
	if(!pi)
		return false;
	return cbl->Register(pi);
}

bool cPluginManager::UnregisterCallBack(string id, cPluginBase *pi)
{
	cCallBackList *cbl = mCallBacks.GetByHash(mCallBacks.Key2Hash(id));
	if(!cbl)
		return false;
	if(!pi)
		return false;
	return cbl->Unregister(pi);
}

void cPluginManager::List(ostream &os)
{
	os << " " << setw(40) << setiosflags(ios::left) << toUpper(_("Name"));
	os << toUpper(_("Version")) << "\r\n";
	os << " " << string(40+25,'=');
	tPlugins::iterator it;
	for (it = mPlugins.begin(); it != mPlugins.end(); ++it) {
		os << "\r\n " << setw(40) << setiosflags(ios::left) << (*it)->mPlugin->Name() << (*it)->mPlugin->Version();
	}
}

void cPluginManager::ListAll(ostream &os)
{
	tCBList::iterator it;
	for (it = mCallBacks.begin(); it != mCallBacks.end(); ++it) {
		os << "\r\nCB: " << (*it)->Name();
		(*it)->ListRegs(os,"\t");
	}
}

cPluginBase * cPluginManager::GetPlugin(const string &Name)
{
	cPluginLoader *pi;
	pi = mPlugins.GetByHash(mPlugins.Key2Hash(Name));
	if(pi)
		return pi->mPlugin;
	else
		return NULL;
}

cPluginBase * cPluginManager::GetPluginByLib(const string &lib)
{
	tPlugins::iterator it;
	for (it = mPlugins.begin(); it != mPlugins.end(); ++it) {
		if ((*it)->GetFilename() == lib) return (*it)->mPlugin;
	}
	return NULL;
}
	}; // namespace nPlugin
}; // namespace nVerliHub
