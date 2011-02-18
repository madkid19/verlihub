/***************************************************************************
 *   Copyright (C) 2004 by Daniel Muller                                   *
 *   dan at verliba dot cz                                                 *
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
#ifndef CPIFORBID_H
#define CPIFORBID_H

#include "src/tlistplugin.h"
#include "cconsole.h"
#include "cforbidden.h"

using namespace nDirectConnect::nPlugin;
using namespace nDirectConnect::nTables;
using namespace nDirectConnect;
using namespace nForbid;
using namespace nConfig;

typedef tpiListPlugin<cForbidden, cForbidConsole> tpiForbidBase;

/**
\brief a forbidden words plugin for verlihub

users may leave offline messages for registered users or reg users may leave offline messages for anyone

@author bourne
@author Daniel Muller (plugin part of it)

*/
class cpiForbid : public tpiForbidBase
{
public:
	cpiForbid();
	virtual ~cpiForbid();
	virtual bool RegisterAll();
	virtual bool OnOperatorCommand(cConnDC *, string *);
	virtual bool OnParsedMsgChat(cConnDC *, cMessageDC *);
	virtual bool OnParsedMsgPM(cConnDC *, cMessageDC *);
	virtual void OnLoad(cServerDC *);
	cForbidCfg *mCfg;
};


#endif
