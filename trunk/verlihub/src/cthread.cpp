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
#include <string>
using namespace std;
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
