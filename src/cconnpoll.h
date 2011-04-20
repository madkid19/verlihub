/***************************************************************************
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
#include "cconnchoose.h"
#ifndef NSERVERCCONNPOLL_H
#define NSERVERCCONNPOLL_H
#if !USE_SELECT
#include <sys/poll.h>
#if !defined _SYS_POLL_H_ && !defined _SYS_POLL_H && !defined pollfd && !defined _POLL_EMUL_H_
/** the poll file descriptor structure (where not defined) */
struct pollfd {
   int fd;           /* file descriptor */
   short events;     /* requested events */
   short revents;    /* returned events */
};
#endif

#include <vector>
using std::vector;

namespace nVerliHub {
	namespace nSocket {

/**
polling connection chooser

@author Daniel Muller
*/
class cConnPoll : public cConnChoose
{
public:
	cConnPoll();
	~cConnPoll();

	/** Calls the poll function to determine non-blocking socket
	  * \sa cConnChoose::Choose
	  */
	virtual int Choose(cTime &tmout)
	{
		return this->poll(tmout.MiliSec());
	};

	/**
	* Register the connection for the given I/O operation.
	* @param conn The connection.
	* @param event Bitwise OR list of I/O operation.
	*/
	virtual void OptIn(tSocket, nEnums::tChEvent);

	/**
	* Unregister the connection for the given I/O operations.
	* @param conn The connection.
	* @param event Bitwise OR list of I/O operation.
	*/
	virtual void OptOut(tSocket, nEnums::tChEvent);

	/**
	* Unregister the connection for the given I/O operation.
	* @param conn The connection.
	* @param event Bitwise OR list of I/O operation.
	*/
	virtual int OptGet(tSocket);
	/// @see cConnChoose::RevGet
	virtual int RevGet(tSocket);
	virtual bool RevTest(tSocket);

	/**
	* Add new connection to be handled by connection manager.
	* @param conn The connection.
	* @return True if connection is added; otherwise false.
	*/
	virtual bool AddConn(cConnBase *);

	/**
	  * Wrapper for pollfd structure. It provides constructor and reset method
	  * @author Daniel Muller
	*/
	struct cPollfd: public pollfd
	{
		cPollfd()
		{
			reset();
		};
		void reset()
		{
			fd=-1;
			events=revents=0;
		};
	};

	virtual bool RevTest(cPollfd &);

	int poll(int wp_sec);
	typedef vector<cPollfd> tFDArray;

	cPollfd &FD(int sock)
	{
		return mFDs[sock];
	}
protected:
	tFDArray mFDs;

	const int mBlockSize;
};

	}; // namespace nSocket
}; // namespace nVerliHub

#endif

#endif
