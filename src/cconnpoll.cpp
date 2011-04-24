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
#include "cconnpoll.h"
#if !USE_SELECT
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

namespace nVerliHub {
	using namespace nEnums;
	namespace nSocket {
cConnPoll::cConnPoll() : mBlockSize(1024)
{
	mFDs.reserve(20480);
}

cConnPoll::~cConnPoll()
{}

void cConnPoll::OptIn(tSocket sock, tChEvent mask)
{
 	unsigned event = FD(sock).events;
	if(!event && mask)
		FD(sock).fd = sock;

	if(mask & eCC_CLOSE)
		FD(sock).events = 0;
	else {
		if(mask & eCC_INPUT)
			event = POLLIN|POLLPRI;
		if(mask & eCC_OUTPUT)
			event |= POLLOUT;
		if(mask & eCC_ERROR)
			event |= POLLERR | POLLHUP | POLLNVAL;
		FD(sock).events |= event;
	}
}

void cConnPoll::OptOut( tSocket sock, tChEvent mask)
{
 	unsigned event = ~(0u);
	if(mask & eCC_INPUT)
		event = ~unsigned(POLLIN|POLLPRI);
	if(mask & eCC_OUTPUT)
		event &= ~unsigned(POLLOUT);
	if(mask & eCC_ERROR)
		event &= ~unsigned(POLLERR | POLLHUP | POLLNVAL);
	if(!(FD(sock).events &= event))
		FD(sock).reset(); // nothing left
}

int cConnPoll::OptGet(tSocket sock)
{
	int mask = 0;
 	unsigned event = FD(sock).events;
	if(!event && (FD(sock).fd == sock))
		mask = eCC_CLOSE;
	else {
		if(event & (POLLIN|POLLPRI))
			mask |= eCC_INPUT;
		if(event & POLLOUT)
			mask |= eCC_OUTPUT;
		if(event & (POLLERR | POLLHUP | POLLNVAL))
			mask |= eCC_ERROR;
	}
	return mask;
}

int cConnPoll::RevGet(tSocket sock)
{
	int mask = 0;
	cPollfd &theFD = FD(sock);
 	unsigned event = theFD.revents;
	if(!theFD.events && (theFD.fd == sock))
		mask = eCC_CLOSE;
	if(event & (POLLIN|POLLPRI))
		mask |= eCC_INPUT;
	if(event & POLLOUT)
		mask |= eCC_OUTPUT;
	if(event & (POLLERR | POLLHUP | POLLNVAL))
		mask |= eCC_ERROR;

	return mask;
}

bool cConnPoll::RevTest(cPollfd &theFD)
{
 	if(theFD.fd == INVALID_SOCKET)
		return false;
	if(!theFD.events)
		return true;
 	unsigned event = theFD.revents;
 	if(!event)
		return false;

	if (event & POLLOUT)
		return true;
	if (event & (POLLIN|POLLPRI))
		return true;
	if (event & (POLLERR | POLLHUP | POLLNVAL))
		return true;
	return false;
}

bool cConnPoll::RevTest(tSocket sock)
{
	cPollfd &theFD = FD(sock);
	return RevTest(theFD);
}

int cConnPoll::poll(int wp_sec)
{
#if ! defined _WIN32
	int ret = 0, n = 0;
	int todo = mFDs.size();
	int done = 0;
	int tmp;
	while(todo) {
		tmp = todo;
		if (tmp > mBlockSize)
			tmp = mBlockSize;
		//TEMP_FAILURE_RETRY( //@todo TEMP_FAILURE_RETRY for the poll
			ret = ::poll(&(mFDs[done]), tmp, wp_sec + 1);
		//);
		if(ret < 0)
			continue;
		todo -= tmp;
		done += tmp;
		n+= ret;
	}
	return n;
#else
	return 0;
#endif
}

bool cConnPoll::AddConn(cConnBase *conn)
{
	if(!cConnChoose::AddConn(conn))
		return false;

	if(mLastSock >= (tSocket)mFDs.size())
		mFDs.resize(mLastSock + mLastSock/2);
	return true;
}

	}; // namepsace nSocket

}; // namespace nVerliHub
#endif

