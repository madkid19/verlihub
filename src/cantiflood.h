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


#ifndef CANTIFLOOD_H
#define CANTIFLOOD_H

#include "cfreqlimiter.h"
#include <string>

using namespace std;

namespace nVerliHub {
	namespace nUtils {
/**antiflood mechanism
  *@author Daniel Muller
  */

class cAntiFlood: public cFreqLimiter
{
public:
	cAntiFlood(double min_delay, double period, long max, long too, string msg, const cTime &now):
		cFreqLimiter(min_delay, period, max, now),
		mFloodMsg(msg),
		mTooCnt(too)
	{};
	~cAntiFlood();
	/** the message to show user when flooding */
	string mFloodMsg;
	long mTooCnt;

	/** returns 0 - ok, -1 - min_delay, -3 = many repeats, -4 too many repeats */
	virtual int Check(const cTime &now)
	{
		int r = cFreqLimiter::Check(now);
		if(r)
		{
			if(mFreq.CountAll(now) > mTooCnt) return -4;
			return r;
		}
		return 0;
	}
};
	}; // namespace nUtils
}; // namespace nVerliHub
#endif
