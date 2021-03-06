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
#ifndef CPIISP_H
#define CPIISP_H

#include "src/tlistplugin.h"
#include "cisps.h"
#include "cconsole.h"
#include "src/cserverdc.h"
#include "src/cconndc.h"
#include "src/cmessagedc.h"

namespace nVerliHub {
	namespace nIspPlugin {

typedef tpiListPlugin<cISPs,cISPConsole> tpiISPBase;

class cpiISP : public tpiISPBase
{
public:
	cpiISP();
	virtual ~cpiISP();
	virtual void OnLoad(nSocket::cServerDC *server);

	virtual bool RegisterAll();
	virtual bool OnParsedMsgMyINFO(nSocket::cConnDC * conn, nProtocol::cMessageDC *mess);
	virtual bool OnParsedMsgValidateNick(nSocket::cConnDC * conn, nProtocol::cMessageDC *mess);
	virtual bool OnOperatorCommand(nSocket::cConnDC *, string *);
	cISPCfg *mCfg;
};
	}; // namespace nIspPlugin
}; // namespace nVerliHub

#endif
