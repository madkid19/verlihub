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
#include <verlihub/ctime.h>
#include <verlihub/cconndc.h>
#include <verlihub/cvhplugin.h>
#include <verlihub/cserverdc.h>

using namespace nDirectConnect::nPlugin;
using namespace nDirectConnect::nTables;
using namespace nDirectConnect;

class cpiFloodprot : public cVHPlugin
{
public:
	cpiFloodprot();
	virtual ~cpiFloodprot();
	virtual void OnLoad(cServerDC *);
	virtual bool RegisterAll();
	virtual bool OnTimer();
	virtual bool OnParsedMsgChat(cConnDC *, cMessageDC *);
	virtual bool OnParsedMsgPM(cConnDC *, cMessageDC *);
	virtual bool OnParsedMsgSearch(cConnDC *, cMessageDC *);
	virtual bool OnParsedMsgMyINFO(cConnDC *, cMessageDC *);
	virtual bool OnNewConn(cConnDC *); 
	virtual bool OnUserLogin(cUser *);
	virtual bool OnUserLogout(cUser *);
	virtual bool OnCloseConn(cConnDC *);
	
	cFloodprotect * mFloodprotect;
};

#endif
