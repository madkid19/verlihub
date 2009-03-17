/***************************************************************************
                          cfreqlimiter.h  -  description
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

#ifndef CFREQLIMITER_H
#define CFREQLIMITER_H

#include "ctimeout.h"
#include "cmeanfrequency.h"

/**
  * Various utility classes, very useful
  *@author Daniel Muller
  */

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

};

#endif
