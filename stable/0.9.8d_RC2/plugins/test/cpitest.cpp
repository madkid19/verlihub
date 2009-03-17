/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cpitest.h"
#include <iostream>

cpiTest::cpiTest()
{
	mName = "TestPlugin1.0";
}

cpiTest::~cpiTest() {
}

bool cpiTest::RegisterAll()
{
	RegisterCallBack("VH_OnNewConn");
	RegisterCallBack("VH_OnCloseConn");
}

void cpiTest::OnLoad(cServerDC* server)
{
	cVHPlugin::OnLoad(server);
	cUser *bot = NewRobot("PokusnejKralik", 10);
	mServer->mP.Create_MyINFO(bot->mMyINFO, bot->mNick, "desc","","mail","0");
	bot->mMyINFO_basic = bot->mMyINFO;
}

bool cpiTest::OnNewConn(cConnDC * conn)
{
	string msg("Hello World!! (this is first verlihub's plugin)");
	mServer->DCPublicHS(msg, conn);
	return true;
}

bool cpiTest::OnCloseConn(cConnDC * conn)
{
	return true;
}

// usual way to do it //
REGISTER_PLUGIN(cpiTest);
