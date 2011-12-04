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
#include "i18n.h"
#include <iostream>

namespace nVerliHub {
	namespace nCmdr {

cCommandCollection::cCommandCollection(void *owner) :
cObj("cCmdr"),
mOwner(owner)
{}

cCommandCollection::~cCommandCollection()
{}

void cCommandCollection::Add(cCommand *command)
{
	if (command) {
		mCmdList.push_back(command);
		command->mCmdr = this;
	}
}

int cCommandCollection::ParseAll(const string &commandLine, ostream &os, void *options)
{
	cCommand *Cmd = this->FindCommand(commandLine);

	if (Cmd != NULL)
		return (int)this->ExecuteCommand(Cmd, os, options);
	else
		return -1;
}

cCommand *cCommandCollection::FindCommand(const string &commandLine)
{
	tCmdList::iterator it;

	for (it = mCmdList.begin(); it != mCmdList.end(); ++it) {
		cCommand *Cmd = *it;
		if (Cmd && Cmd->ParseCommandLine(commandLine)) return Cmd;
	}

	return NULL;
}

bool cCommandCollection::ExecuteCommand(cCommand *command, ostream &os, void *options)
{
	if (command->TestParams()) {
		command->Execute(os, options);
		//if (command->Execute(os, options))
			//os << _("[OK]");
		//else
			//os << _("[ERROR]");

		return true;
	} else {
		os << _("Command parameters error") << ":\r\n";
		command->GetSyntaxHelp(os);
		return false;
	}
}

void cCommandCollection::List(ostream *os)
{
	for (tCmdList::iterator it = mCmdList.begin(); it != mCmdList.end(); ++it) {
		if (*it) {
			(*os) << "\r\n";
			(*it)->Describe(*os);
		}
	}
}

void cCommandCollection::InitAll(void *data)
{
	for (tCmdList::iterator it = mCmdList.begin(); it != mCmdList.end(); ++it) {
		if (*it) (*it)->Init(data);
	}
}
	}; //namespace nCmdr
}; // namespace nVerliHub
