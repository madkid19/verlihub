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
#ifndef NSERVERCCONNSELECT_H
#define NSERVERCCONNSELECT_H
#include "cconnchoose.h"
#if USE_SELECT
#if defined _WIN32
#include "Winsock2.h"
#else
#include <sys/select.h>
#endif
#include "thasharray.h"

namespace nServer {

/**
selecting connection chooser

@author Daniel Muller
*/

class cConnSelect : public cConnChoose
{
public:
	cConnSelect();
	~cConnSelect();

	/** \brief Perform the choose operation
	  *
	  * determine which among the sunscribed connections will not block in specified IO operations
	  * \param tmout timeout for the select operation
	  * \sa OptIn
	  */
	virtual int Choose(cTime &tmout){ return this->Select(tmout); };

	/**
	* Register the connection for the given I/O operation.
	* @param conn The connection.
	* @param event Bitwise OR list of I/O operation.
	*/
	virtual void OptIn(tSocket sock, tChEvent events);
	
	/**
	* Unregister the connection for the given I/O operation.
	* @param conn The connection.
	* @param event Bitwise OR list of I/O operation.
	*/
	virtual void OptOut(tSocket, tChEvent);
	
	/**
	* Return I/O operations for the given connection.
	* @param conn The connection.
	* @return Bitwise OR list of I/O operation.
	* @see OptIn(tSocket sock, tChEvent events)
	*/
	virtual int OptGet( tSocket );
	/// \brief get the result after Choose operation
	virtual int RevGet( tSocket );
	/// \brief test wheather the choose result is non-nul
	virtual bool RevTest( tSocket );

	
	/** 
	* Wrapper for fd_set structure. It provides constructor and = operator.
	* @author Daniel Muller
	*/
	struct sFDSet : public fd_set
	{
		sFDSet(){ FD_ZERO(this);}
		sFDSet & operator=(const sFDSet &set)
		{
			#ifdef _WIN32
			fd_count=set.fd_count;
			memcpy(&fd_array,&(set.fd_array), sizeof (fd_array));
			#else
			memcpy(&fds_bits,&(set.fds_bits), sizeof(fds_bits));
			#endif
			return *this;
		}
		bool IsSet(tSocket sock){ return FD_ISSET(sock, this) != 0;}
		void Clr(tSocket sock){ FD_CLR(sock, this);}
		void Set(tSocket sock){ FD_SET(sock, this);}
	};

	void ClearRevents();
	void FDSet2HashRevents(sFDSet &fdset, unsigned mask);

	typedef tHashArray<sChooseRes *> tFDs;
	struct iterator;

	iterator begin() { return iterator(this, mFDs.begin()); }

	iterator end() { return iterator(this, mFDs.end()); }

	struct iterator
	{
		cConnSelect *mSel;
		tFDs::iterator mIT;
		iterator(){}
		iterator(cConnSelect *sel, tFDs::iterator it) : mSel(sel), mIT(it)
		{}

		iterator & operator=(const iterator &it)
		{
			mSel= it.mSel;
			mIT = it.mIT;
			return *this;
		}

		bool operator != (const iterator &it)
		{
			return mIT != it.mIT;
		}

		sChooseRes & operator *()
		{
			(*mIT)->mConn = (*mSel)[(*mIT)->mSock];
			return *(*mIT);
		}

		bool Advance()
		{
			while (! (++mIT).IsEnd() && !(*mIT)->mRevent){}
			return !mIT.IsEnd();
		}

		iterator & operator ++(){ Advance(); return *this;}
	};

protected:
	 int Select(cTime &tmout);
	// select settings
	sFDSet mReadFS;
	sFDSet mWriteFS;
	sFDSet mExceptFS;
	sFDSet mCloseFS;

	// select results
	sFDSet mResReadFS;
	sFDSet mResWriteFS;
	sFDSet mResExceptFS;

	tFDs mFDs;
};



};

#endif
#endif
