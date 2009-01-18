/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ccommand.h"
#include <iostream>
using namespace std;

namespace nCmdr {

cCommand::cCommand(int ID, const char *IdRegex, const char *ParRegex, sCmdFunc *CmdFunc) :
	mID(ID), mIdentificator(IdRegex, PCRE_ANCHORED),
	mParamsParser(ParRegex, PCRE_DOTALL, 64 ), mCmdFunc(CmdFunc),
	mIdRegexStr(IdRegex),
	mParRegexStr(ParRegex)
{
	mCmdr = NULL;
	if (CmdFunc != NULL)
	{
		CmdFunc->mCommand = this;
		CmdFunc->mIdRex = &mIdentificator;
		CmdFunc->mParRex = &mParamsParser;
	}
}

cCommand::cCommand() :  mCmdr(NULL), mID(-1), mParamsParser(64), mCmdFunc(NULL)
{
}

void cCommand::Init(int ID, const char *IdRegex, const char *ParRegex, sCmdFunc *CmdFunc)
{
	mID = ID;
	mIdentificator.Compile(IdRegex, PCRE_ANCHORED);
	mParamsParser.Compile(ParRegex, PCRE_DOTALL);
	mCmdFunc = CmdFunc;
	mIdRegexStr = IdRegex;
	mParRegexStr = ParRegex;

	mCmdr = NULL;
	if (CmdFunc != NULL)
	{
		CmdFunc->mCommand = this;
		CmdFunc->mIdRex = &mIdentificator;
		CmdFunc->mParRex = &mParamsParser;
	}
}

cCommand::~cCommand(){}

};


/*!
    \fn nCmdr::cCommand::TestID(const string&)
 */
bool nCmdr::cCommand::TestID(const string &str)
{
	int ret = mIdentificator.Exec(str);
	if(ret > 0)
	{
		mIdentificator.Extract(0, str, mIdStr);
		mParStr.assign(str, mIdStr.size(), str.size() - mIdStr.size());
	}
	else
	{
		mIdStr  ="";
		mParStr ="";
	}
	return ret > 0;
}


/*!
    \fn nCmdr::cCommand::TestParams()
 */
bool nCmdr::cCommand::TestParams()
{
	return 0 < mParamsParser.Exec(mParStr);
}


/*!
    \fn nCmdr::cCommand::Execute(ostream &output, void *extrapar)
 */
bool nCmdr::cCommand::Execute(ostream &output, void *extrapar)
{
	mCmdFunc->mIdStr = mIdStr;
	mCmdFunc->mParStr = mParStr;
	return (*mCmdFunc)(mIdentificator, mParamsParser, output, extrapar);
}

/*!
	\fn nCmdr::cCommand::sCmdFunc::StringToIntFromList(const string &str, const char *stringlist[], const int intlist[], int item_count)

 */
int nCmdr::cCommand::sCmdFunc::StringToIntFromList(const string &str, const char *stringlist[], const int intlist[], int item_count)
{
	int theInt = -1;
	int i;

	for(i = 0; i < item_count; i++)
	{
		if (str == stringlist[i])
		{
			theInt = intlist[i];
			return theInt;
		}
	}

	if(theInt == -1)
	{
		(*mOS) << "Sorry, '" << str << "' is not implemented" << endl;
		(*mOS) << "Only known: ";
		for(i = 0; i < item_count; i++) (*mOS) << stringlist[i] << " ";
		(*mOS) << endl;
	}
	return theInt;
}


/*!
    \fn nCmdr::cCommand::ListCommands(ostream &os)
 */
void nCmdr::cCommand::ListCommands(ostream &os)
{
	os << mIdRegexStr << mParRegexStr;
}


/*!
    \fn nCmdr::cCommand::GetParamSyntax(ostream &os)
 */
void nCmdr::cCommand::GetParamSyntax(ostream &os)
{
	mCmdFunc->GetSyntaxHelp(os, this);
}
