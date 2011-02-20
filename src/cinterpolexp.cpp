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
#include "cinterpolexp.h"
#include <iostream>

using std::cout;

namespace nUtils {

cInterpolExp::cInterpolExp(unsigned int &var, long toval, int togo, int skip):
	mVariable(var),
	mTargetValue(toval),
	mStepsToGo(togo),
	mSkipSteps(skip)
{
	cout << " constructor " << endl;
	mInitValue = mVariable;
	mCurrentValue = mInitValue;
	mNextStep = (mTargetValue - mInitValue) /2;
	mSkipedSteps = 0;
}


cInterpolExp::~cInterpolExp()
{
}



/*!
    \fn nUtils::cInterpolExp::step()
 */
void cInterpolExp::step()
{
	cout << " step " << mStepsToGo << "  " << mSkipedSteps << "  "  << mSkipSteps << endl;
	mStepsToGo --;
	if( ( mSkipedSteps++ ) >= mSkipSteps )
	{
		mSkipedSteps = 0;
		mVariable += mNextStep;
		cout << "step " << mNextStep << " value " << mVariable << endl;
		mNextStep /= 2;
	}
}

bool cInterpolExp::done()
{
	return ( mStepsToGo <= 0 );
}

};
