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

#ifndef CPIMESSANGER_H
#define CPIMESSANGER_H

#include "src/cvhplugin.h"
#include "src/ctimeout.h"
#include "src/cdcproto.h"
#include "cconsole.h"
#include "cmsglist.h"

using namespace ::nPlugin;
using namespace ;
using namespace nUtils;
using namespace nMessanger;

/**
\brief a messanger plugin for verlihub

users may leave offline messages for registered users or reg users may leave offline messages for anyone

@author Daniel Muller
*/
class cpiMessanger : public cVHPlugin
{
public:
	cpiMessanger();
	virtual ~cpiMessanger();
	virtual bool RegisterAll();
	virtual bool OnUserCommand(cConnDC *, string *);
	virtual bool OnUserLogin(cUser *);
	virtual bool OnTimer();
	virtual void OnLoad(cServerDC *);
	cConsole mConsole;
	cMsgList * mMsgs;
	cTimeOut mReloadTimer;
};


#endif
