/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NCMDRCCMDR_H
#define NCMDRCCMDR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "cobj.h"
#include "ccommand.h"
#include <vector>
#if defined _WIN32 || HAVE_OSTREAM
#include <ostream>
#else
namespace std{
#include <ostream.h>
};
#endif


using std::vector;
using std::ostream;

/**
\brief Command and their parameter parsing and Interpreting tools
Contains a notion of a Command and a Command list
*/
namespace nCmdr {

/**
a command collection, interprets commands

@author Daniel Muller
*/
class cCmdr: public cObj
{
public:
	cCmdr(void *owner = NULL);
	~cCmdr();
	cCommand *FindCommand(const string &CmdLine);
	int ParseAll(const string &CmdLine, ostream &os, void *extrapar);
	bool ExecuteCommand(cCommand *Cmd, ostream &os, void *extrapar);
	void Add(cCommand *);
	void InitAll(void *);
	void List(ostream *pOS);
	void *mOwner;
private:
	typedef vector<cCommand *> tCmdList;
	tCmdList mCmdList;
};

};

#endif
