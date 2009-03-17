/***************************************************************************
                          cantiflood.h  -  description
                             -------------------
    begin                : Sun Aug 31 2003
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

#ifndef CANTIFLOOD_H
#define CANTIFLOOD_H

#include "cfreqlimiter.h"
#include <string>

using namespace std;
using namespace nUtils;
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

#endif
