/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
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
#ifndef NDIRECTCONNECTCLIENT_H
#define NDIRECTCONNECTCLIENT_H
#include <string>

using namespace std;

namespace nDirectConnect {
	class cConnDC;

	namespace nTables {

		/**
		* This class represents a client with a TAG
		*
		*  @author Simoncelli Davide
		*  @version 1.0
		*/

		class cDCClient
		{
			public:
				/**
				* Class constructor
				*/
				cDCClient();
				
				/**
				* Class destructor
				*/
				virtual ~cDCClient();
				
				/**
				* This function is called when cDCClient object is created. Here it is not useful so the body is empty
				*/
				virtual void OnLoad() {};
				
				/**
				* Redefine << operator to print a client and show its description
				    * @param os The stream where to store the description.
				    * @param tr The cDCClient object that describes the redirect
				    * @return The stream
				*/
				friend ostream &operator << (ostream &, cDCClient &);

				// Name of the client
				string mName;
				
				// Unique string to identufy the client
				string mTagID;
				
				// Min version allowed
				double mMinVersion;
				
				// Max version allowed
				double mMaxVersion;
				
				// Client is banned from the hub
				bool mBan;
				
				//  Enable or disable a client
				int mEnable;
		};
	};
};

#endif
