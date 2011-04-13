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
#include "cpluginloader.h"
#include "i18n.h"

namespace nPlugin {

cPluginLoader::cPluginLoader(const string &filename) :
	cObj("cPluginLoader"),
	mFileName(filename)
{
	mHandle = NULL;
	mError = NULL;
	mPlugin = NULL;
	mcbGetPluginFunc = NULL;
	mcbDelPluginFunc = NULL;
}


cPluginLoader::~cPluginLoader()
{
	if (mHandle) Close();
	if (mPlugin && mcbDelPluginFunc)
	{
		mcbDelPluginFunc(mPlugin);
		mPlugin = NULL;
	}
}
};


/*!
    \fn nPlugin::cPluginLoader::Open()
 */
bool nPlugin::cPluginLoader::Open()
{
	#ifdef _WIN32
	mHandle = LoadLibrary(mFileName.c_str()); 
	if(mHandle == NULL) {
	#else

	#ifdef HAVE_FREEBSD
	/*
	* Reset dlerror() since it can contain error from previous
	* call to dlopen()/dlsym().
	*/
	dlerror();
	#endif

	mHandle = dlopen(mFileName.c_str(), RTLD_NOW);
	if(!mHandle || IsError()) // Note that || operator evaluates only the first statement if that one is true
	{
		if (!mHandle) IsError(); // Call it again
	#endif
		if(ErrLog(1)) LogStream() << autosprintf(_("Cannot open plugin '%s' beacause: %s"),mFileName.c_str(),Error().c_str()) << endl;
		return false;
	}
	return true;
}


/*!
    \fn nPlugin::cPluginLoader::Close()
 */
bool nPlugin::cPluginLoader::Close()
{
	mcbDelPluginFunc(mPlugin);
	mPlugin = NULL;
	#ifdef _WIN32
	if(!FreeLibrary(mHandle))
	#else
	dlclose(mHandle);
	if(IsError())
	#endif
	{
		if(ErrLog(1)) LogStream() << autosprintf(_("Cannot close plugin because: %s"),Error().c_str()) << endl;
		return false;
	}
	mHandle = NULL;
	return true;
}

/** log the event */
int nPlugin::cPluginLoader::StrLog(ostream & ostr, int level)
{
	if(cObj::StrLog(ostr,level))
	{
		LogStream()   << "(" << mFileName << ") ";
		return 1;
	}
	return 0;
}


/*!
    \fn nPlugin::cPluginLoader::LoadSym()
 */
bool nPlugin::cPluginLoader::LoadSym()
{
	#ifdef HAVE_FREEBSD
	/*
	* Reset dlerror() since it can contain error from previous
	* call to dlopen()/dlsym().
	*/
	dlerror();
	#endif
	if(!mcbGetPluginFunc) mcbGetPluginFunc = tcbGetPluginFunc(LoadSym("get_plugin"));
	if(!mcbDelPluginFunc) mcbDelPluginFunc = tcbDelPluginFunc(LoadSym("del_plugin"));
	if(!mcbGetPluginFunc|| !mcbGetPluginFunc) return false;
	return (mPlugin = mcbGetPluginFunc()) != NULL;
	return true;
}


/*!
    \fn nPlugin::cPluginLoader::LoadSym(const char *)
 */
void * nPlugin::cPluginLoader::LoadSym(const char *name)
{
	#ifdef _WIN32
	void *func = (void *) GetProcAddress(mHandle, name);
	if(func == NULL) {
		if(ErrLog(1)) LogStream() << autosprintf(_("Cannot load '%s' exported interface because: %s"),name,GetLastError()) << endl;
		return NULL;
	}
	#else
	void *func = dlsym( mHandle, name);
	if(IsError())
	{
		if(ErrLog(1)) LogStream() <<  autosprintf(_("Cannot load '%s' exported interface because: %s"),name,Error().c_str()) << endl;
		return NULL;
	}
	#endif
	return func;
}
