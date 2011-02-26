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
#ifndef NUTILS_CTIMEOUT_H
#define NUTILS_CTIMEOUT_H
#include "ctime.h"

namespace nUtils
{
/**mesures timeout and detects flooding
  *@author Daniel Muller
  */

/**
 * Minimum delay and maximum delay controler
 * */

class cTimeOut {
public: 
	cTimeOut():mMinDelay(0l),mMaxDelay(0l),mLast(0l){};
	cTimeOut(double min,double max,const cTime &now):mMinDelay(min),mMaxDelay(max),mLast(now){};
	~cTimeOut();

public:
   void SetMinDelay(double mind) { mMinDelay = mind;}
   void SetMaxDelay(double maxd) { mMaxDelay = maxd;}
public: // Public attributes
	/** minimum delay between events */
	cTime mMinDelay;
	/** maximum delay between events */
	cTime mMaxDelay;
	/** last event time*/
	cTime mLast;

	/**
	 * Checks the validity, return 0 on sucess; -1 on timein; -2 on timeout
	 * if event is positive, the last event time is updated
	 * */
	int Check(const cTime &now, long event=0)
	{
		if ( !bool(mLast)) return 0;
		cTime diff(now);
		// check min delay
		if( event && (bool)mMinDelay)
		{
			diff -= mLast;
			if(mMinDelay > diff) return -1;
		}

		// check the max delay
		if( bool(mMaxDelay) && (mMaxDelay < (diff = now - mLast)) ) return -2;

		// update time of last event
		if(event) Reset(now);

		// ok np
		return 0;
	}

	/** Reset the events to now */
	void Reset(const cTime & now){ mLast = now;};
	void Disable(){ mLast= 0.;}

};

};
#endif
