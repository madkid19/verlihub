/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "tpluginbase.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif

namespace nPlugin {

tPluginBase::tPluginBase():cObj("PluginBase"), mHandle(NULL)
{}

tPluginBase::~tPluginBase()
{}

};


/*!
    \fn nPlugin::tPluginBase::Open()
 */
bool nPlugin::tPluginBase::Open()
{
	mHandle = dlopen(mFileName.c_str(), RTLD_NOW);
	if(!mHandle)
	{
		if(ErrLog(1)) LogStream() << "Can't open file '" << mFileName
			<< "' because:" << Error() << endl;
		return false;
	}
	return true;
}


/*!
    \fn nPlugin::tPluginBase::Close()
 */
bool nPlugin::tPluginBase::Close()
{
	if(dlclose(mHandle)){
		if(ErrLog(1)) LogStream() << "Can't close :" << Error() << endl;
	}
	return true;
}


/*!
    \fn nPlugin::tPluginBase::Error()
 */
string nPlugin::tPluginBase::Error()
{
	const char *error = dlerror();
	return string(error?error:"ok");
}

/** log the event */
int nPlugin::tPluginBase::StrLog(ostream & ostr, int level)
{
	if(cObj::StrLog(ostr,level))
	{
		LogStream()   << "(" << mName << ") ";
		return 1;
	}
	return 0;
}
