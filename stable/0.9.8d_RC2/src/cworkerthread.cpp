/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cworkerthread.h"

namespace nThreads
{

cWorkerThread::cWorkerThread() : mWork(NULL)
{}


cWorkerThread::~cWorkerThread()
{
	Stop(true);
	if (mWork)
	{
		delete mWork;
		mWork = NULL;
	}

}

bool cWorkerThread::AddWork(cThreadWork *theWork)
{
	bool Result = false;
	if (TryLock())
	{
		if (!mWork)
		{
			mWork = theWork;
			Result = true;
		}
		UnLock();
	}
	if(Result) Start();
	return Result;
}

bool cWorkerThread::HasSomethingToDo()
{
	return mWork != NULL;
}

/*!
    \fn nThreads::cWorkerThread::Thread(cObj *)
 */
void cWorkerThread::DoSomething()
{
	if (mWork != NULL)
	{
		mWork->DoTheWork();
		delete mWork;
		mWork = NULL;
	}
}

};


