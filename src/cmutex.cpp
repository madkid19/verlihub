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
