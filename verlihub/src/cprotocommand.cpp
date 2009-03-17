/***************************************************************************
                          cprotocommand.cpp  -  description
                             -------------------
    begin                : Mon May 12 2003
    copyright            : (C) 2003 by Daniel Muller
    email                : dan at verliba dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "cprotocommand.h"
#include "stringutils.h"
using namespace nStringUtils;

namespace nServer
{

cProtoCommand::cProtoCommand()
{
}

cProtoCommand::cProtoCommand(string cmd)
	: mCmd ( cmd )
{
	mBaseLength = mCmd.length();
}

cProtoCommand::~cProtoCommand()
{
}

/** test if str is of this command */
bool cProtoCommand::AreYou(const string &str)
{
	return 0==StrCompare(str,0,mCmd.length(),mCmd);
}

};
