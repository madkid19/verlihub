/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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
	#ifdef _WIN32
	mHandle = LoadLibrary(mFileName.c_str()); 
	if(mHandle == NULL)
	#else
      
	#ifdef HAVE_FREEBSD
	/*
	* Reset dlerror() since it can contain error from previous
	* call to dlopen()/dlsym().
	*/
	dlerror();
	#endif

	mHandle = dlopen(mFileName.c_str(), RTLD_NOW);
	if(!mHandle)
	#endif
	{
		if(ErrLog(1)) LogStream() << "Can't open plugin '" << mFileName << "' because:" << Error() << endl;
		return false;
	}
	return true;
}


/*!
    \fn nPlugin::tPluginBase::Close()
 */
bool nPlugin::tPluginBase::Close()
{
	#ifdef _WIN32
	if(!FreeLibrary(mHandle))
	#else
	if(dlclose(mHandle))
	#endif
	{
		if(ErrLog(1)) LogStream() << "Can't close :" << Error() << endl;
	}
	return true;
}


/*!
    \fn nPlugin::tPluginBase::Error()
 */
string nPlugin::tPluginBase::Error()
{
	const char *error;
	#ifdef _WIN32
		LPVOID buff;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
			mHandle,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &buff,
			0,
			NULL 
		);
		error= (const char *) buff;
		LocalFree(buff);
	#else
	error = dlerror();
	#endif
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
