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

#include "cpireplace.h"
#include "src/cserverdc.h"

using namespace nDirectConnect;

cpiReplace::cpiReplace() : mConsole(this), mReplacer(NULL), mCfg(NULL)
{
	mName = "WordReplacer";
	mVersion = REPLACER_VERSION;
}

cpiReplace::~cpiReplace()
{
	if(mReplacer)
		delete mReplacer;
	mReplacer = NULL;
	if(mCfg)
		delete mCfg;
	mCfg = NULL;
}


void cpiReplace::OnLoad(cServerDC *server)
{
	cVHPlugin::OnLoad(server);
	mReplacer = new cReplacer(server);
	mReplacer->CreateTable();
	mReplacer->LoadAll();
	mCfg = new cReplaceCfg(mServer);
	mCfg->Load();
	mCfg->Save();
}

bool cpiReplace::RegisterAll()
{
	RegisterCallBack("VH_OnOperatorCommand");
	RegisterCallBack("VH_OnParsedMsgChat");
	return true;
}

bool cpiReplace::OnParsedMsgChat(cConnDC *conn, cMessageDC *msg)
{
	string & text = msg->ChunkString(eCH_CH_MSG);
	text = mReplacer->ReplacerParser(text, conn);

	msg->ApplyChunk(eCH_CH_MSG);

	return true;
}

bool cpiReplace::OnOperatorCommand(cConnDC *conn, string *str)
{
	if(mConsole.DoCommand(*str, conn))
		return false;
	return true;
}

REGISTER_PLUGIN(cpiReplace);
