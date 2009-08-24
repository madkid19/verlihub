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
#ifndef NUTILS_CMEANFREQUENCY_H
#define NUTILS_CMEANFREQUENCY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ctime.h"
#if HAVE_STRING_H
#include <string.h>
#endif
#include <iostream>

using namespace std;

namespace nUtils {
/**
  *@author Daniel Muller
  */

/**
 * Mesuers a frequency over a given period of incomming events
 * events are counted by type T, they are summed
 * the given interval is divined into several subintervals
 * to make better resolution
 * */

template<class T, int max_size=20> class cMeanFrequency
{
	public:
	/** period length over which it is measured */
	cTime mOverPeriod;
	/** length of every part or resolution */
	cTime mPeriodPart;
	/** start and end of mesured time */
	cTime mStart, mEnd;
	/** start (maybe end) of the partly period where we are now */
	cTime mPart;
	/** resolution of mesure */
	int mResolution;
	/** counts of events in every part of pertiod/resolution */
	T mCounts[max_size];
	int mStartIdx;
	/** number of filled periods */
	int mNumFill;

	void Dump(void)
	{
		cout << "mOverPeriod: " << mOverPeriod.AsPeriod()
			<< " mStart, mEnd: " << mStart.AsDate() << " " << mEnd.AsDate()
			<< " mPart, mPeriodPart " << mPart.AsDate() << " " << mPeriodPart.AsPeriod()
			<< " mResolution:" << mResolution
			<< " mCounts[" ;
		for (int i = 0; i < max_size; i++) cout << mCounts[i] << ", ";
		cout << "] " << "mStartIdx:" << mStartIdx << " mNumFill:" << mNumFill << endl;
	}

	/************** conctruction */
	cMeanFrequency()
	{
		cTime now;
		mResolution = max_size;
		SetPeriod(0.);
		mResolution = max_size;
		Reset(now);
	}

	cMeanFrequency(const cTime &now)
	{
		mResolution = max_size;
		SetPeriod(1.);
		Reset(now);
	};

	cMeanFrequency(const cTime &now, double per, int res):
		mOverPeriod(per),
		mPeriodPart(per/res),
		mResolution(res)
	{
		Reset(now);
	};

	/** insert/add */
	void Insert(const cTime &now, T data=1)
	{
		Adjust(now);
		mCounts[(mStartIdx+mNumFill) % mResolution] += data;
	};

	double GetMean(const cTime &now)
	{
		T sum = CountAll(now);
		double Sum = sum;
		if (! mNumFill) return 0.;
		Sum *= mResolution / mNumFill;
		Sum /= double(mOverPeriod);
		return Sum;
	}

	/** calculate count over all period */
	T CountAll(const cTime &now)
	{
		Adjust(now);
		T sum=0;
		int i,j=mStartIdx;
		for(i=0; i < mNumFill; i++)
		{
			sum += mCounts[j++];
			if(j >= mResolution) j = 0;
		}
		return sum;
	};

	/** adjust state to current time */
	void Adjust(const cTime &now)
	{
		// need adjustment
		if( mEnd < now )
		{
			cTime t(mEnd);
			t += mOverPeriod; // if last adjustment happend more thern period ago
			// in this case we can empty all
			if( t < now ) { Reset(now); return; }
			// shift until it's ok
			while(mEnd < now) Shift();
		}
		else if( mNumFill < mResolution)
		{
			while(( mPart < now ) && ( mNumFill < mResolution))
			{
				mPart += mPeriodPart;
				mNumFill ++;
			}
		}
	};

	// left (remove first part period)
	void Shift()
	{
		mEnd   += mPeriodPart;
		mStart += mPeriodPart;
		mCounts[mStartIdx] = 0;
		if(mNumFill > 0) mNumFill --;
		mStartIdx ++;
		if (mStartIdx >= mResolution) mStartIdx -= mResolution;
	};

	void Reset(const cTime &now)
	{
		memset(&mCounts,0, sizeof(mCounts));
		mStart = now;
		mEnd =  mStart;
		mEnd += mOverPeriod;
		mNumFill = 0;
		mStartIdx = 0;
		mPart = mStart;
		mPart += mPeriodPart;
	}

	/** setup the opeverperiod and the resolution */
	void SetPeriod(double per)
	{
		mOverPeriod = cTime(per);
		mPeriodPart = mOverPeriod / mResolution;
	};

};

};

#endif
