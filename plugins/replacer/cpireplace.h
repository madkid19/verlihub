/***************************************************************************
 *   Copyright (C) 2004 by Pralcio                                         *
 *   based on "Forbid" code made by                                        *
 *   Daniel Muller                                                         *
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
#ifndef CPIREPLACE_H
#define CPIREPLACE_H

#include "src/cvhplugin.h"
#include "cconsole.h"
#include "creplacer.h"

using namespace nDirectConnect::nPlugin;
using namespace nDirectConnect::nTables;
using namespace nDirectConnect;
using namespace nReplace;

/**
\brief a plugin that replaces chosen words with other in main chat of DC
@author bourne
@author Daniel Muller (plugin part of it)
@author Pralcio
*/
class cpiReplace : public cVHPlugin
{
public:
	cpiReplace();
	virtual ~cpiReplace();
	virtual bool RegisterAll();
	virtual bool OnOperatorCommand(cConnDC *, string *);
	virtual bool OnParsedMsgChat(cConnDC *, cMessageDC *);
	virtual void OnLoad(cServerDC *);
	cConsole mConsole;
	cReplacer *mReplacer;
	cReplaceCfg *mCfg;
};


#endif
