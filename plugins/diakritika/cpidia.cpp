/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cpidia.h"
#include "cdiacfg.h"
#include <iostream>

cpiDia::cpiDia()
{
	mName = "Diakritika";
	mVersion = "1.0.1";
	mCfg = NULL;
}

/** \brief Called once when being loaded by verlihub
  */
void cpiDia::OnLoad(cServerDC* server)
{
	cVHPlugin::OnLoad(server);
	mCfg = new cDiaCfg(mServer);
	mCfg->Load();

	int pos = 0, max = 0 ;
	for(pos=0; pos < 256; pos++)replace_array[pos]=char(pos);
	max = mCfg->find_set.size();
	if (max > mCfg->replace_set.size()) max = mCfg->replace_set.size();
	for(pos=0; pos < max ; pos++)
		replace_array[(unsigned char)(mCfg->find_set[pos])]=mCfg->replace_set[pos];
}

cpiDia::~cpiDia() 
{
	if( mCfg) delete mCfg;
}

bool cpiDia::RegisterAll()
{
	RegisterCallBack("VH_OnParsedMsgChat");
}

bool cpiDia::OnParsedMsgChat(cConnDC * conn, cMessageDC *msg)
{
	if (!mCfg || !mCfg->find_set.size()) return true;
	int pos = 0;
	string &message = msg->ChunkString(eCH_CH_MSG);
	unsigned char old_char, new_char;
	while (pos != message.npos)
	{
		pos = message.find_first_of(mCfg->find_set, pos);
		if(pos != message.npos)
		{
			old_char = message[pos];
			new_char = this->replace_array[old_char];
			message.replace(pos,1,1,new_char);
			pos++;
		}
	}
	msg->ApplyChunk(eCH_CH_MSG);
	return true;
}


REGISTER_PLUGIN(cpiDia);
