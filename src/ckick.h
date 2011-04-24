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
#ifndef CKICK_H
#define CKICK_H
#include <string>
using namespace std;
#if (!defined _WIN32) && (!defined __int64)
#define __int64 long long
#endif

namespace nVerliHub {
	namespace nTables {
/**this represents kick by ops, has all information, that mey serve to ban
  *@author Daniel Muller
  */

class cKick {
public:
	cKick();
	~cKick();
	string mNick;
	string mReason;
	long mTime;
	string mIP;
	string mOp;
	bool mIsDrop;
	string mEmail;
	string mHost;
	__int64 mShare;
};

	}; // namespace nTables
}; //namespace nVerliHub
#endif
