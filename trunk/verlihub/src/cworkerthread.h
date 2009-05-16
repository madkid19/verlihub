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
