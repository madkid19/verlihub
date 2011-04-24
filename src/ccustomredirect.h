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
#ifndef NDIRECTCONNECTCREDIRECT_H
#define NDIRECTCONNECTCREDIRECT_H
#include <sstream>
#include <string>

using namespace std;

namespace nVerliHub {
	namespace nEnums {
		enum {
			eKick = 0x000001,
			eUserLimit = 0x000002,
			eShareLimit = 0x00004,
			eTag = 0x000008,
			eWrongPasswd = 0x000010,
			eInvalidKey = 0x000020,
			eHubBusy = 0x000040,
			eReconnect =  0x000080
		};
	};

	namespace nTables {

		/**
		* cRedirect class represent a redirect.
		*
		* @author Simoncelli Davide
		* @version 1.0
		*/

		class cRedirect
		{
			public:
				/**
				* Class constructor
				*/
				cRedirect();

				/**
				* Class destructor
				*/
				virtual ~cRedirect();

				/**
				* This function is called when cRedirect object is created. Here it is not useful so the body is empty
				*/
				virtual void OnLoad() {};

				/**
				* Redefine << operator to print a redirect and show its status
				* @param os The stream where to store the description.
				* @param tr The cRedirect object that describes the redirect
				* @return The stream
				*/
				friend ostream &operator << (ostream &, cRedirect &);

				// The flag to determinate the kind of the redirect
				int mFlag;

				// The target
				string mAddress;

				// Enable or disable redirect
				int mEnable;
		};
	}; // namespace nTables
}; // namespace nVerliHub

#endif
