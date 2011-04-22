/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2011 by Verlihub Project                           *
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
#ifndef NTHREADSCTHREAD_H
#define NTHREADSCTHREAD_H
#include "cmutex.h"
#include "cobj.h"

namespace nVerliHub {
	namespace nThread {
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
	}; // namespace nThread
}; // namespace nVerliHub


#endif
