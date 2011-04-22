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

#ifndef CASYNCSOCKETSERVER_H
#define CASYNCSOCKETSERVER_H

#include "cconnchoose.h"

#if USE_SELECT
#include "cconnselect.h"
#else
#include "cconnpoll.h"
#endif
#include "ctimeout.h"
#include <list>
#include "cobj.h"
//#include "cconndc.h" // added
#include "casyncconn.h"
#include "cmeanfrequency.h"

using namespace std;

namespace nVerliHub {
	using namespace nUtils;
	namespace nSocket {
/**
 * General asynchronous (non-blocking) socket server with some basic functionality.
 *
 * @author Daniel Muller
 */
class cAsyncSocketServer : public cObj
{
public:
	friend class nVerliHub::nSocket::cAsyncConn;

	/**
	* Class constructor.
	* Create a server and start listening on given port.
	* @param port The port to listen on.
	*/
	cAsyncSocketServer(int port=0);

	/**
	* Class destructor.
	*/
	virtual ~cAsyncSocketServer();

	/**
	* Stop main process loop and delete all connections.
	*/
	void close();

	/**
	* Return the port on which the server is listening on.
	* @return The port.
	*/
	virtual unsigned int getPort() const;

	/**
	* This event is triggered when a connection is closed.
	* @param conn Closed connection.
	*/
	void OnConnClose(cAsyncConn*);

	/**
	* This event is triggered every period of time.
	* @param now Current time.
	* @return The result.
	*/
	virtual int OnTimer(cTime &now);

	/**
	* This event is trigger every N seconds and triggers the event OnTimerBase for every connections.
	* @param now Current time.
	*/
	int OnTimerBase(cTime &now);

	/**
	* Main process loop. Run it until it is stopped or paused.
	* @return The error code.
	*/
	int run();

	/**
	* Stop main process loop and set error code.
	* @param code The error code.
	*/
	void stop(int);

	/**
	* Time step.
	* This method accepts new incoming connection, take care of existing ones and close connections that are not actived anymore.
	*/
	void TimeStep();

	virtual cAsyncConn * ListenWithConn(cAsyncConn *, int OnPort, bool UDP=false);
	virtual cAsyncConn * Listen(int OnPort, bool UDP = false);
	virtual bool StopListenConn(cAsyncConn *);
	virtual int StartListening(int OverrideDefaultPort=0);
	/** Write property of int mPort. */
	virtual void setPort(const unsigned int _newVal);

	// Listening address
	string mAddr;
	// The port to listen on
	unsigned int mPort;
	// Connection period for timer
	int timer_conn_period;
	// Server period for timer
	int timer_serv_period;
	// Delay in milliseconds for every step
	int mStepDelay;
	// Max size of chucks when message is splitted
	unsigned long mMaxLineLength;
	// Reverse DNS lookup when user joins
	int mUseDNS;
	// Current time
	cTime mTime;
	// Measure the frequency of the server
	nUtils::cMeanFrequency<unsigned ,21> mFrequency;

    protected:
	// Indicate if the loop should be run or not
	bool mbRun;

	typedef list<cAsyncConn*> tConnList;
	// Iterator to iterate over connections list
	typedef tConnList::iterator tCLIt;
	// List of connections (pointers to cAsyncConn class)
	tConnList mConnList;
	#if !USE_SELECT
		cConnPoll mConnChooser;
	#else
		cConnSelect mConnChooser;
	#endif

	// True if Windows sockets is initialized
	static bool WSinitialized;
	cConnFactory *mFactory;

	/**
	* Add the connection to the server so it can be processed every period of time.
	* @param conn The connection to add.
	*/
	virtual void addConnection(cAsyncConn *);

	/** return true if accept is allowed */
	/**
	* Return true if the server accepts new incoming connection.
	* @return True if the server accepts a new connection or false otherwise.
	*/
	virtual bool AllowNewConn() { return true; };

	/**
	* Remove the connection from the server.
	* The pointer to the connection will be deleted and not valid anymore.
	* @param conn The connection to remove.
	*/
	void delConnection(cAsyncConn *);

	/**
	* Create a new string buffer for input/ouput operation.
	* @param conn The connection.
	* @return Pointer to the new string.
	*/
	virtual string * FactoryString(cAsyncConn *);

	/**
	* Perform input operation on the given connection.
	* This method will read all data from the connection
	* and return the number of read bytes or a negative number if an error occurred.
	* @param conn The connection.
	* @return Number of read bytes.
	*/
	virtual int input(cAsyncConn *conn);

	/**
	* This event is triggered when there is a new incoming message
	* from a single connection.
	* @param conn The connection that has new incoming message.
	* @param message Pointer to the new message
	*/
	virtual void OnNewMessage(cAsyncConn *, string *);

	/**
	* Perform output operation on the given connection.
	* This method will send all data from the connection
	* and return the number of sent bytes or a negative number if an error occurred.
	* @param conn The connection.
	* @return Number of sent bytes.
	*/
	int output(cAsyncConn * conn);

	/**
	* This event is triggered when there is a new incoming connection.
	* @param conn The new connection.
	* @return Zero if connection is accepted or a negative number otherwise.
	*/
	virtual int OnNewConn(cAsyncConn*);

	// Structure that contains timers
	struct sTimers
	{
		// Main timer
		cTime main;
		// Connection timer
		cTime conn;
	};
	// Timer structure
	sTimers mT;

	// Errore code when main loop ends
	int mRunResult;
    private:
	// Pointer to the connection that server is currently handling
	cAsyncConn * mNowTreating;


};
	}; // namespace nSocket
}; // namespace nVerliHub
#endif
