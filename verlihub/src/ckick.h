/***************************************************************************
                          ckick.h  -  description
                             -------------------
    begin                : Fri Jul 4 2003
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

#ifndef CKICK_H
#define CKICK_H

#include <string>

using namespace std;
#if (!defined _WIN32) && (!defined __int64)
#define __int64 long long
#endif

namespace nDirectConnect {
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

};
};
#endif
