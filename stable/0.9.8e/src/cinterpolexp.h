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
