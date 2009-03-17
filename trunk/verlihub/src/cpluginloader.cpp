/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cpluginloader.h"

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
	mHandle = dlopen(mFileName.c_str(), RTLD_NOW);
	if(!mHandle || IsError()) // NOTE hte OR (||) operator evaluates only the first statement if that one is true
	{
		if (!mHandle) IsError();// ... that's why here the second is called again
		if(ErrLog(1)) LogStream() << "Can't open file '" << mFileName
			<< "' because:" << Error() << " handle(" << mHandle << ")" << endl;
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
	dlclose(mHandle);
	if(IsError())
	{
		if(ErrLog(1)) LogStream() << "Can't close :" << Error() << endl;
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
	void *func = dlsym( mHandle, name);
	if(IsError())
	{
		if(ErrLog(1)) LogStream() << "Can't load " << name <<" exported interface :" << Error() << endl;
		return NULL;
	}
	return func;
}
