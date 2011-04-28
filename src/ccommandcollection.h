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
	/// @addtogroup Plugin
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
			cCommandCollection(void *owner = NULL);

			/**
			 * Class destructor
			 */
			~cCommandCollection();

			cCommand *FindCommand(const string &CmdLine);
			int ParseAll(const string &CmdLine, ostream &os, void *extrapar);
			bool ExecuteCommand(cCommand *Cmd, ostream &os, void *extrapar);
			void Add(cCommand *);
			void InitAll(void *);
			void List(ostream *pOS);
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
