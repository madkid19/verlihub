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
#ifndef CPROTOCOMMAND_H
#define CPROTOCOMMAND_H
#include <string>

using namespace std;
namespace nVerliHub {
	namespace nProtocol {

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

	}; // namespace nProtocol
}; // namespace nVerliHub
#endif
