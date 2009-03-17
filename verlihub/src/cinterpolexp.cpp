/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cinterpolexp.h"
#include <iostream>

using std::cout;

namespace nUtils {

cInterpolExp::cInterpolExp(int &var, long toval, int togo, int skip):
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
