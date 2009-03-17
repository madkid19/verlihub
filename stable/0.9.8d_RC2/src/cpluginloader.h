/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NPLUGINCPLUGINLOADER_H
#define NPLUGINCPLUGINLOADER_H

#include <string>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#include "cpluginbase.h"

using std::string;

namespace nPlugin {

/**
class for encapsulating plugins to be loaded

@author Daniel Muller
*/
class cPluginLoader : public cObj
{
public:
	cPluginLoader(const string &filename);
	~cPluginLoader();
 	bool Open();
	bool Close();
	bool LoadSym();
	int StrLog(ostream & ostr, int level);
	bool IsError(){return (mError = dlerror()) != NULL;}
	string Error(){return string((mError!=NULL)?mError:"ok");}
	string GetFilename(){ return mFileName;}
	void * LoadSym(const char *);
	cPluginBase *mPlugin;

protected:
	string mFileName;
	const char * mError;
	void *mHandle;
	typedef cPluginBase *(*tcbGetPluginFunc)(void);
	typedef void (*tcbDelPluginFunc)(cPluginBase *);
	tcbDelPluginFunc mcbDelPluginFunc;
	tcbGetPluginFunc mcbGetPluginFunc;
};

};

#endif