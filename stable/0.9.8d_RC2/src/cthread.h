/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   ... looked a bit on Mathen's code in dclib                            *
 ***************************************************************************/
#ifndef NTHREADSCTHREAD_H
#define NTHREADSCTHREAD_H

#include "cmutex.h"
#include "cobj.h"

namespace nThreads
{

/**
@author Daniel Muller
*/
class cThread : public cMutex
{
public:
	cThread();
	virtual ~cThread();

	/** start thread */
	int Start();
	/** stop it */
	int Stop( bool bHard = true );
	/** is it stopped ? */
	bool Stopped() const;

	/** sleep N nanoseconds */
	void NanoSleep( unsigned int N);

	/** */
	typedef void * (*tThreadFunc)(void*);
	static void* ThreadFunc(void *);

	/** */
	virtual bool HasSomethingToDo() = 0;
	virtual void DoSomething() = 0;

protected:
	/** */
	int mRun;
	/** */
	pthread_t mThread;
	/** */
	int mStop;
};
};


#endif
