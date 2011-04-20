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
#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#define BUFSIZE MAX_PATH
#include <local.h>
#endif
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "cserverdc.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <signal.h>
#include <dirent.h>
#include "script_api.h"
#include "i18n.h"
#include "dirsettings.h"

using namespace std;
using namespace nVerliHub;
using namespace nVerliHub::nSocket;

#if ! defined _WIN32
void mySigPipeHandler(int i)
{
	signal(SIGPIPE,mySigPipeHandler);
	cout << "Received SIGPIPE, ignoring it, " << i << endl;
}

void mySigIOHandler(int i)
{
	signal(SIGIO  ,mySigIOHandler  );
	cout << endl << "Received SIGIO, ignoring it, " << i << endl;
}

void mySigQuitHandler(int i)
{
	cout << "Received a " << i << " signal, quiting";
	exit(0);
}

void mySigHupHandler(int i)
{
	signal(SIGPIPE,mySigHupHandler);
	cout << "Received a " << i << " signal";
	cServerDC *Hub = (cServerDC *)cServerDC::sCurrentServer;
	if (Hub) Hub->mC.Load();
}

#endif

bool DirExists(const char *dirname)
{
	DIR *dir ;
	dir = opendir(dirname);
	if( dir == NULL)
	{
		return false;
	}
	else
	{
		closedir(dir);
		return true;
	}
}

int main(int argc, char *argv[])
{
	int result = 0;
	string ConfigBase;

	#ifdef _WIN32
	TCHAR Buffer[BUFSIZE];
	if(!GetCurrentDirectory(BUFSIZE, Buffer)) {
		cout << "Cannot get current directory because: " << GetLastError() << endl;
		return 1;
	}
	ConfigBase = Buffer;
	#else
	const char *DirName = NULL;
	char *HomeDir = getenv("HOME");
	string tmp;
	if (HomeDir) {
		tmp = HomeDir;
		tmp +=  "/.verlihub";
		DirName = tmp.c_str();
		if (DirExists(DirName))
			ConfigBase = DirName;
	}
	DirName = "./.verlihub";
	if (DirExists(DirName))
		ConfigBase = DirName;
	DirName = getenv("VERLIHUB_CFG");
	if ((DirName != NULL) && DirExists(DirName))
		ConfigBase = DirName;
	if (!ConfigBase.size()) {
		ConfigBase = "/etc/verlihub";
	}
	#endif
	cout << "Config dir " << ConfigBase << endl;
	cServerDC server(ConfigBase, argv[0]);
	setlocale(LC_ALL, server.mDBConf.locale.c_str());
	bindtextdomain("verlihub", LOCALEDIR);
	textdomain("verlihub");

	int port=0;

	if(argc > 1) {
		stringstream arg(argv[1]);
		arg >> port;
	}
	#ifndef _WIN32
	signal(SIGPIPE,mySigPipeHandler);
	signal(SIGIO  ,mySigIOHandler  );
	signal(SIGQUIT,mySigQuitHandler);
	#endif

	server.StartListening(port);
	result = server.run(); // run the main loop until it stops itself
	return result;
}
