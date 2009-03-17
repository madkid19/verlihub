/***************************************************************************
                          cprotocommand.h  -  description
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

#ifndef CPROTOCOMMAND_H
#define CPROTOCOMMAND_H


#include <string>

using namespace std;

namespace nServer
{

/** \brief DC protocol command
  * provides a Method Telling wheter a given command is this one or not
  * description of a command from a protocol, about command, ans syntax, all parameters etc...
  * @author Daniel Muller
  */

class cProtoCommand
{
public:
	/** std constructor and destructor */
	cProtoCommand();
	cProtoCommand(string cmd);
	virtual ~cProtoCommand();
	/** test if str is of this command */
	bool AreYou(const string &str);
public: // Public attributes
	/** the command keyword */
	string mCmd;
	/** the length of the basic command string */
	int mBaseLength;
};

};
#endif
