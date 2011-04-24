/***************************************************************************
 *   Copyright (C) 2004 by Janos Horvath                                   *
 *   bourne@freemail.hu                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef _CPIFLOODPROT_H_
#define _CPIFLOODPROT_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "cfloodprotect.h"
#include "src/ctime.h"
#include "src/cconndc.h"
#include "src/cvhplugin.h"
#include "src/cmessagedc.h"
#include "src/cserverdc.h"

namespace nVerliHub {
	namespace nFloodProtectPlugin {
class cpiFloodprot : public nPlugin::cVHPlugin
{
public:
	cpiFloodprot();
	virtual ~cpiFloodprot();
	virtual void OnLoad(cServerDC *);
	virtual bool RegisterAll();
	virtual bool OnTimer();
	virtual bool OnParsedMsgChat(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgPM(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgSearch(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnParsedMsgMyINFO(nSocket::cConnDC *, nProtocol::cMessageDC *);
	virtual bool OnNewConn(nSocket::cConnDC *);
	virtual bool OnUserLogin(cUser *);
	virtual bool OnUserLogout(cUser *);
	virtual bool OnCloseConn(nSocket::cConnDC *);

	cFloodprotect * mFloodprotect;
};
	}; // namespace nFloodProtectPlugin
}; // namespace nVerliHub
#endif
