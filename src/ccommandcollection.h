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
#ifndef NCMDRCCMDR_H
#define NCMDRCCMDR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "cobj.h"
#include "ccommand.h"
#include <vector>
#include <ostream>

using std::vector;
using std::ostream;

namespace nVerliHub {
	namespace nCmdr {
		/// @addtogroup Command
		/// @{
		/**
		* A command interpreter collection.
		* This class provides basic functionality to run commands
		* with their options.
		* @author Daniel Muller
		*/
		class cCommandCollection: public cObj
		{
			public:
				/**
				* Class constructor.
				* @param owner The owner of this instance.
				*/
				cCommandCollection(void *owner = NULL);

				/**
				* Class destructor
				*/
				~cCommandCollection();

				/**
				* Add the given command to the collection.
				* @param command The command to add.
				*/
				void Add(cCommand *command);

				/**
				* Execute the given command and options.
				* @param command The command to execute.
				* @param os The stream where to store the result.
				* @param options The options of the command.
				* @return True if the command has been executed or false otherwise.
				* @see ParseAll()
				*/
				bool ExecuteCommand(cCommand *command, ostream &os, void *options);

				/**
				* Find a command in the collection.
				* @param commandLine The command with its options.
				* @return An instance of cCommand class.
				*/
				cCommand *FindCommand(const string &commandLine);

				/**
				 * Initialize all commands in the collection
				 * with the given data.
				 * @param data The data to initialize the commands.
				 */
				void InitAll(void *data);

				/**
				 * Output the list of commands in the collection
				 * in the given stream.
				 * @param os The stream where to store the result.
				 */
				void List(ostream *os);

				/**
				* Parse the command line and run the found command.
				* @param commandLine The command line.
				* @param os The stream where to store the result.
				* @param options The options of the command.
				* @return -1 if the command cannot be found,
				* 0 if the command cannot be executed or 1 otherwise.
				* @see ExecuteCommand()
				* @todo Rename to ExecuteCommandLine()
				*/
				int ParseAll(const string &commandLine, ostream &os, void *options);

				/// The owner of command collection.
				void *mOwner;
			private:
				/// Define a list of commands.
				typedef vector<cCommand *> tCmdList;
				/// The list of commands
				tCmdList mCmdList;
			};
		/// @}
	}; // namespace nCmdr
}; // namespace nVerliHub

#endif
