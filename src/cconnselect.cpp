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
#include "cconnselect.h"
#if ! HAVE_SYS_POLL_H
namespace nServer {

cConnSelect::cConnSelect()
{
}


cConnSelect::~cConnSelect()
{
	tFDs::iterator it;
	sChooseRes *FD;
	tSocket sock;
	for (it = mFDs.begin(); it != mFDs.end(); )
	{
		FD = *it;
		++it;

		if (FD)
		{
			sock = FD->mSock;
			delete FD;
			mFDs.RemoveByHash(sock);
		}
	}
}

void cConnSelect::OptIn ( tSocket sock, tChEvent mask)
{
	if( mask & eCC_INPUT  ) mReadFS.Set(sock);
	if( mask & eCC_OUTPUT ) mWriteFS.Set(sock);
	if( mask & eCC_ERROR  ) mExceptFS.Set(sock);
	if( mask & eCC_CLOSE  ) mCloseFS.Set(sock);
	sChooseRes *FD = mFDs.GetByHash(sock);
	if (!FD)
	{
		FD = new sChooseRes;
		FD->mSock = sock;
		FD->mEvent = mask;
		mFDs.AddWithHash(FD , sock );
	}
	else
	{
		FD->mEvent |= mask;
	}

}

void cConnSelect::OptOut( tSocket sock, tChEvent mask)
{
	if( mask & eCC_INPUT  ) mReadFS.Clr(sock);
	if( mask & eCC_OUTPUT ) mWriteFS.Clr(sock);
	if( mask & eCC_ERROR  ) mExceptFS.Clr(sock);
	if( mask & eCC_CLOSE  ) mCloseFS.Clr(sock);
	sChooseRes *FD = mFDs.GetByHash(sock);
	if(FD)
	{
		FD->mEvent -= (FD->mEvent & mask);
		if(!FD->mEvent)
		{
			delete FD;
			mFDs.RemoveByHash(sock);
		}
	}
}

int cConnSelect::OptGet( tSocket sock )
{
	int mask = 0;
	if( mReadFS.IsSet(sock) ) mask |= eCC_INPUT;
	if( mWriteFS.IsSet(sock)) mask |= eCC_OUTPUT;
	if( mExceptFS.IsSet(sock))mask |= eCC_ERROR;
	if( mCloseFS.IsSet(sock) )mask |= eCC_CLOSE;
	return mask;
}

int cConnSelect::RevGet( tSocket sock )
{
	int mask = 0;
	if( mResReadFS.IsSet(sock) ) mask |= eCC_INPUT;
	if( mResWriteFS.IsSet(sock)) mask |= eCC_OUTPUT;
	if( mResExceptFS.IsSet(sock))mask |= eCC_ERROR;
	if( mCloseFS.IsSet(sock)    )mask |= eCC_CLOSE;
	return mask;
}

bool cConnSelect::RevTest( tSocket sock )
{
	if( mResWriteFS.IsSet(sock)) return true;
	if( mResReadFS.IsSet(sock) ) return true;
	if( mResExceptFS.IsSet(sock))return true;
	if( mCloseFS.IsSet(sock)    )return true; // note that's not an error
	return false;
}

int cConnSelect::Select( cTime &tmout )
{
	mResReadFS = mReadFS;
	mResWriteFS = mWriteFS;
	mResExceptFS = mExceptFS;
	int size = mLastSock;//mFDs.size();
	int ret = select(size, &mResReadFS, &mResWriteFS, &mResExceptFS, (timeval *)(&tmout));
	if( ret == SOCKET_ERROR ) return -1;
	ClearRevents();
	FDSet2HashRevents(mResReadFS, eCC_INPUT);
	FDSet2HashRevents(mResWriteFS, eCC_OUTPUT);
	FDSet2HashRevents(mResExceptFS, eCC_ERROR);
	FDSet2HashRevents(mCloseFS, eCC_CLOSE);
	return ret;
}

void cConnSelect::ClearRevents(void)
{
	tFDs::iterator it;
	for (it = mFDs.begin(); it != mFDs.end(); ++ it)
		if(*it) (*it)->mRevent = 0;
}


void cConnSelect::FDSet2HashRevents(sFDSet &fdset, unsigned mask)
{
	unsigned i;
	tSocket sock;
	#ifdef _WIN32
	for(i = 0; i < fdset.fd_count; i++)
	{
		sock = fdset.fd_array[i];
		sChooseRes *FD = mFDs.GetByHash(sock);
		if (!FD)
		{
			FD = new sChooseRes;
			FD->mSock = sock;
			FD->mEvent = 0;
			FD->mRevent = mask;
			mFDs.AddWithHash(FD , sock );
		}
		else
		{
			FD->mRevent |= mask;
		}
	}
	#else
	for(i = 0; i < FD_SETSIZE; i++)
	{
		sock = i;
		if (FD_ISSET(sock,&fdset))
		{
			sChooseRes *FD = mFDs.GetByHash(sock);
			if (!FD)
			{
				FD = new sChooseRes;
				FD->mSock = sock;
				FD->mEvent = 0;
				FD->mRevent = mask;
				mFDs.AddWithHash(FD , sock );
			}
			else
			{
				FD->mRevent |= mask;
			}
		}
	}
	#endif
}

};
#endif

