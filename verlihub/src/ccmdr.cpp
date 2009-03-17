/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ccmdr.h"

namespace nCmdr {

cCmdr::cCmdr(void *owner):cObj("cCmdr"), mOwner(owner){}
cCmdr::~cCmdr(){}
};



/*!
    \fn nCmdr::cCmdr::ParseAll(const string &CmdLine, ostream &os, void *extrapar)
    \return the number of command found..
 */
int nCmdr::cCmdr::ParseAll(const string &CmdLine, ostream &os, void *extrapar)
{
	cCommand *Cmd = this->FindCommand(CmdLine);
	if (Cmd != NULL) return (int)this->ExecuteCommand(Cmd, os, extrapar);
	else return -1;
}

nCmdr::cCommand *nCmdr::cCmdr::FindCommand(const string &CmdLine)
{
	tCmdList::iterator it;
	for(it = mCmdList.begin(); it != mCmdList.end(); ++it)
	{
		cCommand *Cmd = *it;
		if( Cmd && Cmd->TestID(CmdLine))
			return Cmd;
	}
	return NULL;
}

bool nCmdr::cCmdr::ExecuteCommand(cCommand *Cmd, ostream &os, void *extrapar)
{
	if( Cmd->TestParams() )
	{
		if(Cmd->Execute(os, extrapar)) os << "OK";
		else os << "ERROR";
		return true;
	}
	else
	{
		os << "Params error.." << "\r\n";
		Cmd->GetParamSyntax(os);
		return false;
	}
}

void nCmdr::cCmdr::List(ostream *pOS)
{
	tCmdList::iterator it;
	for(it = mCmdList.begin(); it != mCmdList.end(); ++it) 
	{
		if (*it)
		{
			(*it)->ListCommands(*pOS);
			(*pOS) << "\r\n";
		}
	}
}

/*!
    \fn nCmdr::cCmdr::Add(cCommand *)
 */
void nCmdr::cCmdr::Add(cCommand *cmd)
{
	if(cmd)
	{
		//mCmdList.reserve(cmd->mID+1);
		mCmdList.push_back(cmd);
		cmd->mCmdr = this;
	}
}


/*!
    \fn nCmdr::cCmdr::InitAll(void *)
 */
void nCmdr::cCmdr::InitAll(void *par)
{
	tCmdList::iterator it;
	for(it = mCmdList.begin(); it != mCmdList.end(); ++it) if (*it) (*it)->Init(par);
}
