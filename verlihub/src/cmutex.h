/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NTHREADSCMUTEX_H
#define NTHREADSCMUTEX_H

#include <pthread.h>

namespace nThreads
{

/** mutual lock

@author Daniel Muller
*/
class cMutex
{
public:
	cMutex();
	~cMutex();
	int Lock();
	int UnLock();
	bool TryLock();
	void ClearError();
	const char* GetError();
private:
	void PrintError(const char *function);
	pthread_mutex_t mMutex;
	int mLastError;
};

};

#endif
