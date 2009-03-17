/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NTHREADSCWORKERTHREAD_H
#define NTHREADSCWORKERTHREAD_H

#include "cthread.h"
#include "cthreadwork.h"

namespace nThreads
{

/**
a thread designed to do some work of type cThreadWork

@author Daniel Muller
*/
class cWorkerThread : public cThread
{
public:
	cWorkerThread();
	virtual ~cWorkerThread();

	/**
	 * \brief Queue some work
	 * \return fals when no more work can be queud
	 * */
	virtual bool AddWork(cThreadWork *);

	/**
	 * \brief to know if there is work to do
	 *
	 * thread asks itself if not, it sleeps, or also other thread may ask this, to make it run
	 * */
	virtual bool HasSomethingToDo();

	/**
	 * \bried Does a single piece of work
	 *
	 * and when it's done, it removes the work
	 * */
	virtual void DoSomething();

protected:
	cThreadWork * mWork;
};

};

#endif
