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

#ifndef CPIIPLOG_H
#define CPIIPLOG_H

#include "src/cvhplugin.h"
#include "cconsole.h"
#include "ciplog.h"

namespace nVerliHub {
	namespace nIPLogPlugin {
/**
\brief a statistics plugin for verlihub

users may leave offline messages for registered users or reg users may leave offline messages for anyone

@author Daniel Muller
*/
class cpiIPLog : public nPlugin::cVHPlugin
{
public:
	cpiIPLog();
	virtual ~cpiIPLog();
	virtual bool RegisterAll();
	virtual bool OnOperatorCommand(nSocket::cConnDC *, string *);
	virtual bool OnNewConn(nSocket::cConnDC * conn);
	virtual bool OnCloseConn(nSocket::cConnDC *conn);
	virtual void OnLoad(nSocket::cServerDC *);
	virtual bool OnUserLogin(cUser *);
	virtual bool OnUserLogout(cUser *);
	cConsole mConsole;
	cIPLog * mIPLog;
	int mLogFlags;
};
	}; // namespace using namespace ::nPlugin;
}; // namespace nVerliHub

#endif
