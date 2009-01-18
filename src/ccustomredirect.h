/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
