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
#include "ccommand.h"
#include "i18n.h"
#include <iostream>
using namespace std;

namespace nVerliHub {
	namespace nCmdr {

cCommand::cCommand(int ID, const char *regexId, const char *paramRegex, sCmdFunc *CmdFunc) :
mID(ID), mIdentificator(regexId, PCRE_ANCHORED),
mParamsParser(paramRegex, PCRE_DOTALL, 64), mCmdFunc(CmdFunc),
mIdRegexStr(regexId),
mParStr(paramRegex)
{
	mCmdr = NULL;
	if(CmdFunc != NULL) {
		CmdFunc->mCommand = this;
		CmdFunc->mIdRex = &mIdentificator;
		CmdFunc->mParRex = &mParamsParser;
	}
}

cCommand::cCommand() :
mCmdr(NULL),
mID(-1),
mParamsParser(64),
mCmdFunc(NULL)
{
}

void cCommand::Init(int ID, const char *regexId, const char *paramRegex, sCmdFunc *CmdFunc)
{
	mID = ID;
	mIdentificator.Compile(regexId, PCRE_ANCHORED);
	mParamsParser.Compile(paramRegex, PCRE_DOTALL);
	mCmdFunc = CmdFunc;
	mIdRegexStr = regexId;
	mParRegexStr = paramRegex;

	mCmdr = NULL;
	if(CmdFunc != NULL) {
		CmdFunc->mCommand = this;
		CmdFunc->mIdRex = &mIdentificator;
		CmdFunc->mParRex = &mParamsParser;
	}
}

cCommand::~cCommand()
{}

bool cCommand::ParseCommandLine(const string &str)
{
	int ret = mIdentificator.Exec(str);
	if(ret > 0) {
		mIdentificator.Extract(0, str, mIdStr);
		mParStr.assign(str, mIdStr.size(), str.size() - mIdStr.size());
	} else {
		mIdStr = "";
		mParStr = "";
	}
	return ret > 0;
}

bool cCommand::TestParams()
{
	return 0 < mParamsParser.Exec(mParStr);
}

bool cCommand::Execute(ostream &output, void *extrapar)
{
	mCmdFunc->mIdStr = mIdStr;
	mCmdFunc->mParStr = mParStr;
	return (*mCmdFunc)(mIdentificator, mParamsParser, output, extrapar);
}

int cCommand::sCmdFunc::StringToIntFromList(const string &str, const char *stringlist[], const int intlist[], int item_count)
{
	int theInt = -1;
	int i;

	for(i = 0; i < item_count; i++) {
		if(str == stringlist[i]) {
			theInt = intlist[i];
			return theInt;
		}
	}

	if(theInt == -1) {
		(*mOS) << autosprintf(_("Command suffix %s is not implemented, known suffixes are"), str.c_str()) << ":";
		for(i = 0; i < item_count; i++)
			(*mOS) << " " << stringlist[i];
	}
	return theInt;
}

void cCommand::Describe(ostream &os)
{
	os << _("Suffix regular expression") << ": " << mIdRegexStr << "\t" << _("Parameters regular expression") << ": " << mParRegexStr;
}

void cCommand::GetSyntaxHelp(ostream &os)
{
	mCmdFunc->GetSyntaxHelp(os, this);
}

	}; // namespae nCmdr
}; // namespace nVerliHub
