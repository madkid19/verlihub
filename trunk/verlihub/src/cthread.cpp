/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <unistd.h>
#include "cthread.h"


namespace nThreads
{

cThread::cThread()
{
	mStop = 1;
	mRun  = 0;
}

cThread::~cThread()
{
	void * status;

	mStop = 1;

	if ( mRun == 1 )
	{
		pthread_join(mThread, &status);
	}
}

bool cThread::Stopped() const
{
	return (mStop == 1) || (mRun == 0);
}

/** */
int cThread::Start()
{
	if (mRun || !mStop) return -1;
	mStop = 0;

	return pthread_create(&mThread, 0, (tThreadFunc)ThreadFunc, this);
}

/** */
int cThread::Stop( bool BeHard )
{
	void * status;

	if (!mRun || mStop) return -1;
	mStop=1;

	if ( BeHard ) pthread_join(mThread,&status);
	return 0;
}

void *cThread::ThreadFunc(void *obj)
{
	cThread *This = (cThread *)obj;
	This->mRun = 1;
	while(!This->mStop) // infinite lop, can be stopped only by deleting instance
	{
		if  (This->HasSomethingToDo())
		{
			This->DoSomething();
		}
		else
		{
			usleep(1000);
		}
	}
	This->mRun = 0;
	return obj;
}

};
