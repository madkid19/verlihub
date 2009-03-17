/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NTHREADSCTHREADWORK_H
#define NTHREADSCTHREADWORK_H

namespace nThreads
{

/**
a definition of work what a WorkerThread should do..
a base class
@author Daniel Muller
*/

class cThreadWork
{
public:
	cThreadWork();
	virtual ~cThreadWork();
	/**
	 * \brief Thread will call this without parameters
	 * \return 0 on success, otherwise error code
	 **/
	virtual int DoTheWork()=0;
};

template<class ClassType, class Typ1, class Typ2, class Typ3> class tThreadWork3T : public cThreadWork
{
public :
	typedef int (ClassType::*tT3Callback)(Typ1, Typ2, Typ3);
	tThreadWork3T(Typ1 const &par1, Typ2 const & par2, Typ3 const & par3, ClassType *object, tT3Callback cb) : mCB(cb), mObject(object), mPar1(par1), mPar2(par2), mPar3(par3)
	{
	}

	virtual int DoTheWork()
	{
		return (mObject->*mCB)(mPar1, mPar2, mPar3);
	}
protected:
	tT3Callback mCB;
	ClassType * mObject;
	Typ1 mPar1;
	Typ2 mPar2;
	Typ3 mPar3;
};
};


#endif
