/***************************************************************************
                          casyncsocketserver.h  -  description
                             -------------------
    begin                : Tue Jun 10 2003
    copyright            : (C) 2003 by Daniel Muller
    email                : dan at verliba dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cconnchoose.h"

#ifndef CASYNCSOCKETSERVER_H
#define CASYNCSOCKETSERVER_H


#if USE_SELECT
#include "cconnselect.h"
#else
#include "cconnpoll.h"
#endif
#include "ctimeout.h"
#include <list>
#include "cobj.h"
#include "casyncconn.h"
#include "cmeanfrequency.h"

using namespace std;
using namespace nUtils;

/**
  *  General Purpose socket server, hopefully easy to use and with some basic functionality
  *
*/
namespace nServer {


/**reusable asynchronous (rather non-blocking) socket server, multi-client, general purpose
  *@author Daniel Muller
  */
class cAsyncSocketServer : public cObj
{
public:
	friend class cAsyncConn;
	virtual ~cAsyncSocketServer();
	/** create a server listening on the given port */
	cAsyncSocketServer(int port=0);
	/** do one time step, accept incomming connection, take care of existing, close closed connections */
	//void step();
	/** runs the main loop while protected mbRun is true, that is set by stop() function or pause function */
	int run();
	/** stop the main loop, do the last step and end it, doesn't close any connection */
	void stop(int);
	/** close the server with all connections, stop the loop and free all stuff */
	void close();
	virtual cAsyncConn * ListenWithConn(cAsyncConn *, int OnPort, bool UDP=false);
	virtual cAsyncConn * Listen(int OnPort, bool UDP = false);
	virtual bool StopListenConn(cAsyncConn *);
	virtual int StartListening(int OverrideDefaultPort=0);
	/** listen and bind server to localhost address on port defined by SetListenPort function */
	//virtual int listen(int port=0, bool udp=false);
	/** Write property of int mPort. */
	virtual void setmPort( const int& _newVal);
	/** Read property of int mPort. */
	virtual const int& getmPort();
	/** trigger is called when connection is closed */
	void OnConnClose(cAsyncConn*);
	/** this function is going to be executed periodicaly every N seconds and will call function of the same name in evey connection */
	int OnTimerBase(cTime &now);
	/** this is called every period of time */
	virtual int OnTimer(cTime &now);
	/** do one time step, accept incomming connection, take care of existing, close closed connections */
	void TimeStep();
	/** the selector for sockets*/
	//cSelector mSelector;
	/** listen address */
	string mAddr;
	/** period of connections and server timer */
	int timer_conn_period;
	int timer_serv_period;
	/** delay in miliseconds on every step */
	int mStepDelay;
	/** max size of every piece of message */
	unsigned long mMaxLineLength;
	//typedef cSelector::iterator tSelectorKeyIterator;
	/** fund hostnames for connections */
	int mUseDNS;
	/** the current time */
	cTime mTime;
	//
	cMeanFrequency<unsigned ,21> mFrequency;
protected: // Protected attributes
	/** indicates whether the loop should run or not */
	bool mbRun;
	/** the listening socket */
	//REM cAsyncConn mServerSock;
	/** the server listenning port */
	int mPort;
	/** the list of connection pointers to cAsyncConn class or inherited */
	typedef list<cAsyncConn*> tConnList;
	typedef tConnList::iterator tCLIt;
	tConnList mConnList;
	#if !USE_SELECT
		cConnPoll mConnChooser;
	#else
		cConnSelect mConnChooser;
	#endif

	/* a windows socket inicialisation only once*/
	static bool WSinitialized;
	cConnFactory *mFactory;
protected: // Protected methods
	/** add given connection to server */
	virtual void addConnection(cAsyncConn *);
	/** remove given connection from server */
	void delConnection(cAsyncConn * );
	/** perform input operation, read all data from the connection, return number of bytes read, return negative number, if error occured */
	virtual int input(cAsyncConn *conn);
	/** perform output operation */
	int output(cAsyncConn * conn);
	/** treat message for given connection */
	virtual void OnNewMessage(cAsyncConn *, string *);
	/** create somehow a string to get line for given connection, ad return th pointer */
	virtual string * FactoryString(cAsyncConn *);
	/** return true if accept is allowed */
	virtual bool AllowNewConn(){return true;};
	/** return negative if conn should be removed */
	virtual int OnNewConn(cAsyncConn*);

	/// container for connestion timers
	struct sTimers
	{
		// maion timer
		cTime main;
		// connection timer
		cTime conn;
	};
	sTimers mT;
private: // Private attributes
	/** this won't be deleted instatnly if asked */
	cAsyncConn * mNowTreating;
protected:
	int mRunResult;
};
};

#endif
