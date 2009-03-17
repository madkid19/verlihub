/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NUTILSCINTERPOLEXP_H
#define NUTILSCINTERPOLEXP_H

#include "cconfigbase.h"
#include "ctempfunctionbase.h"
#include <string>

using std::string;

namespace nUtils {

/**
a Utility to interpolate a value of a "cConfig' variable between two time events, expecting the increment function will be called in regular intervals during given period

@author Daniel Muller
*/
class cInterpolExp : public cTempFunctionBase
{
public:
	cInterpolExp( int &var, long toval, int skip, int togo);
	~cInterpolExp();

	/** the function to call at every time step
	increases (reduces) the value of a variable
	*/
	public: virtual void step();
	public: virtual bool done();

protected:

	int & mVariable;

	/** values for orientation*/
	long mTargetValue;
	long mInitValue;
	long mCurrentValue;

	/** number of steps to do, and number of steps to skip */
	int mStepsToGo;
	int mSkipSteps;
	int mSkipedSteps;

	long mNextStep;

};

};

#endif
