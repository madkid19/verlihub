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
#ifndef NPLUGINCPLUGINLOADER_H
#define NPLUGINCPLUGINLOADER_H
#include <string>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <dlfcn.h>
#ifdef _WIN32
#include <windows.h>
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
	bool IsError()
	{
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
		mError= (const char *) buff;
		LocalFree(buff);
		#else
		return (mError = dlerror()) != NULL;
		#endif
	}
	string Error(){return string((mError!=NULL)?mError:"ok");}
	string GetFilename(){ return mFileName;}
	void * LoadSym(const char *);
	cPluginBase *mPlugin;

protected:
	string mFileName;
	const char * mError;
	#ifdef _WIN32
	HINSTANCE mHandle;
	#else
	void *mHandle;
	#endif
	typedef cPluginBase *(*tcbGetPluginFunc)(void);
	typedef void (*tcbDelPluginFunc)(cPluginBase *);
	tcbDelPluginFunc mcbDelPluginFunc;
	tcbGetPluginFunc mcbGetPluginFunc;
};

};

#endif
