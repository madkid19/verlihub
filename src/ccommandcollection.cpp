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
#include "ccommandcollection.h"

namespace nVerliHub {
	namespace nCmdr {

cCommandCollection::cCommandCollection(void *owner):cObj("cCmdr"), mOwner(owner)
{}

cCommandCollection::~cCommandCollection()
{}

int cCommandCollection::ParseAll(const string &CmdLine, ostream &os, void *extrapar)
{
	cCommand *Cmd = this->FindCommand(CmdLine);
	if(Cmd != NULL)
		return (int)this->ExecuteCommand(Cmd, os, extrapar);
	else
		return -1;
}

cCommand *cCommandCollection::FindCommand(const string &CmdLine)
{
	tCmdList::iterator it;
	for(it = mCmdList.begin(); it != mCmdList.end(); ++it) {
		cCommand *Cmd = *it;
		if( Cmd && Cmd->TestID(CmdLine))
			return Cmd;
	}
	return NULL;
}

bool cCommandCollection::ExecuteCommand(cCommand *Cmd, ostream &os, void *extrapar)
{
	if(Cmd->TestParams()) {
		if(Cmd->Execute(os, extrapar))
			os << " OK";
		else
			os << "Error";
		return true;
	} else {
		os << "Params error.." << "\r\n";
		Cmd->GetParamSyntax(os);
		return false;
	}
}

void cCommandCollection::List(ostream *pOS)
{
	tCmdList::iterator it;
	for(it = mCmdList.begin(); it != mCmdList.end(); ++it) {
		if(*it) {
			(*it)->ListCommands(*pOS);
			(*pOS) << "\r\n";
		}
	}
}

void cCommandCollection::Add(cCommand *cmd)
{
	if(cmd) {
		//mCmdList.reserve(cmd->mID+1);
		mCmdList.push_back(cmd);
		cmd->mCmdr = this;
	}
}

void cCommandCollection::InitAll(void *par)
{
	tCmdList::iterator it;
	for(it = mCmdList.begin(); it != mCmdList.end(); ++it)
		if(*it)
			(*it)->Init(par);
}

	}; //namespace nCmdr
}; // namespace nVerliHub