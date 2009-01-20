/***************************************************************************
*   Copyright (C) 2009 by Simoncelli Davide                               *
*   netcelli@verlihub-project.org                                         *
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

namespace nDirectConnect {
	namespace nEnums {
		enum {
			eKick = 0x000001,
			eUserLimit = 0x000002,
			eShareLimit = 0x00004,
			eTag = 0x000008,
			eWrongPasswd = 0x000010
		};
	};
	class cConnDC;
	class cServerDC;

	namespace nTables {

/**
		a trigger command ...
		user defined string that triggers given action

		@author Daniel Muller
 */

		class cRedirect
		{
			public:
				cRedirect();
				virtual ~cRedirect();

				int mFlag;
				string mAddress;
				int mEnable;
				virtual void OnLoad();
				friend ostream &operator << (ostream &, cRedirect &);
		};
	};
};

#endif
