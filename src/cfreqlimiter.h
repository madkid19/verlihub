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
#ifndef CFREQLIMITER_H
#define CFREQLIMITER_H
#include "ctimeout.h"
#include "cmeanfrequency.h"

/**
  * Various utility classes, very useful
  *@author Daniel Muller
  */
namespace nVerliHub {
	namespace nUtils {

/** a simple frequency limitter
 * You can setup:
 * 	minimum delay between two events
 * 	maximum frequency mesured over a given time period
 * If Any is not verified, you know it.
 *@author Daniel Muller
 *
 */

class cFreqLimiter
{
public:
	cFreqLimiter(double min_f, double period, long max_cnt, const cTime & now):
		mTmOut(min_f,0,now),
		mFreq(now,period,5),
		mMaxCnt(max_cnt)
	{};

	void SetParams(double min_f, double period, long max_cnt)
	{
		mTmOut.SetMinDelay(min_f);
		mFreq.SetPeriod(period);
		mMaxCnt = max_cnt;
	}

	cFreqLimiter();
	virtual ~cFreqLimiter();

	/** to mesure min delay */
	cTimeOut mTmOut;
	/** to mesure frequency */
	cMeanFrequency<long> mFreq;
	/** the frequency limiter */
	long mMaxCnt;

	/** return 0 if ok, -1 if short delay, -3 if high frequency */
	virtual int Check(const cTime &now)
	{
		int r = mTmOut.Check(now,1);
		if(r) return r;
		mFreq.Insert(now,1);
		if(mFreq.CountAll(now) > mMaxCnt) return -3;
		return 0;
	}
};

	}; // namespace nUtils
}; // namespace nVerliHub

#endif
