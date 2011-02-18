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
#ifndef CPISTATS_H
#define CPISTATS_H

#include "src/cvhplugin.h"
#include "src/ctimeout.h"
#include "src/cmeanfrequency.h"
#include "cstats.h"

using namespace nDirectConnect::nPlugin;
using namespace nDirectConnect;
using namespace nUtils;
using namespace nStats;

/**
\brief a statistics plugin for verlihub

users may leave offline messages for registered users or reg users may leave offline messages for anyone

@author Daniel Muller
*/
class cpiStats : public cVHPlugin
{
public:
	cpiStats();
	virtual ~cpiStats();
	virtual bool RegisterAll();
//	virtual bool OnUserCommand(cConnDC *, string *);
	virtual bool OnParsedMsgSearch(cConnDC *, cMessageDC *);
	virtual void OnLoad(cServerDC *);
	virtual bool OnTimer();
	//cConsole mConsole;
	cStats * mStats;
private:
	cTimeOut mStatsTimer;
	cMeanFrequency<int> mFreqSearchA;
	cMeanFrequency<int> mFreqSearchP;
};


#endif
