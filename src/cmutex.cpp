/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan@verliba.cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cmutex.h"
#include <iostream>
#include <string.h>

using namespace std;

namespace nThreads
{

cMutex::cMutex()
{
	if ( (mLastError=pthread_mutex_init(&mMutex,NULL)) != 0 )
		PrintError(__FUNCTION__);
}

cMutex::~cMutex()
{
	if ((mLastError=pthread_mutex_destroy(&mMutex)) != 0 )
		PrintError(__FUNCTION__);
}

int cMutex::Lock()
{
	if ((mLastError=pthread_mutex_lock(&mMutex)) != 0 )
		PrintError(__FUNCTION__);
	return 0;
}

int cMutex::UnLock()
{
	if ((mLastError=pthread_mutex_unlock(&mMutex)) != 0 )
		PrintError(__FUNCTION__);
	return 0;
}

bool cMutex::TryLock()
{
	return mLastError=pthread_mutex_trylock(&mMutex) == 0;
}


void cMutex::ClearError()
{
	mLastError = 0;
}

const char* cMutex::GetError()
{
	return strerror(mLastError);
}

void cMutex::PrintError(const char *function)
{
	cerr << "Mutex error in " << function << " : " << GetError() << endl;
}

};
