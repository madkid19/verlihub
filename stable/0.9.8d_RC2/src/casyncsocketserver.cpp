/***************************************************************************
                          casyncsocketserver.cpp  -  description
                             -------------------
    begin                : Tue Jun 10 2003
    copyright            : (C) 2003 by dan at verliba dot cziel Muller
    email                : dan at verliba dot cz@verliba.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "casyncsocketserver.h"

#if defined _WIN32
#  include <Winsock2.h>
#else
#  include <sys/socket.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

using namespace std;
using namespace nUtils;

namespace nServer {

bool cAsyncSocketServer::WSinitialized = false;

/** create a server listening on the given port */
cAsyncSocketServer::cAsyncSocketServer(int port):
	cObj("cAsyncSocketServer"),
	mAddr("0.0.0.0"),
	timer_conn_period(4),
	timer_serv_period(2),
	mStepDelay(0),
	mMaxLineLength(10240),
	mUseDNS(0),
	mFrequency(mTime, 90.0, 20),
	mPort(port),
	mFactory(NULL),
	mNowTreating(NULL),
	mRunResult(0)
{
#ifdef _WIN32
	if(!this->WSinitialized)
	{

		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD( 2, 2 );

		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			return;
		}

		/* Confirm that the WinSock DLL supports 2.2.*/
		/* Note that if the DLL supports versions greater    */
		/* than 2.2 in addition to 2.2, it will still return */
		/* 2.2 in wVersion since that is the version we      */
		/* requested.                                        */

		if ( LOBYTE( wsaData.wVersion ) != 2 ||
			HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return;
		}

		/* The WinSock DLL is acceptable. Proceed. */
		this->WSinitialized = true;

	}
#endif
}

cAsyncSocketServer::~cAsyncSocketServer()
{
	close();
#ifdef _WIN32
	WSACleanup();
#endif
	cout << "Allocated objects: " << cObj::GetCount() << endl;
	cout << "Unclosed sockets: " << cAsyncConn::sSocketCounter << endl;
}

/** runs the main loop while protected mbRun is true, that is set by stop() function or pause function */
int cAsyncSocketServer::run()
{
	mbRun = true;
	cTime now;
	if(Log(1)) LogStream() << "Main loop start." << endl;
	while(mbRun)
	{
		mTime.Get();
		//try
		{
			TimeStep();
		}
		//catch(const char *str){
		//	if(ErrLog(2)) LogStream() << "Exception: " << str << endl;}
		//catch(...){
		//	throw "  cAsyncSocketServer::run()";}
		//onEveryStepInRun();

		if(now.Get() >= (mT.main + timer_serv_period))
		{
			mT.main = now;
			OnTimerBase(now);
		}
		#if !defined _WIN32
		::usleep(mStepDelay*1000);
		#else
		//::Sleep(mStepDelay);
		#endif
		mFrequency.Insert(mTime);
	}
	if(Log(1)) LogStream() << "Main loop stop(" << mRunResult <<")." << endl;
	return mRunResult;
}

/** stop the main loop, do the last step and end it, doesn't close any connection */
void cAsyncSocketServer::stop(int code)
{
	mbRun = false;
	mRunResult = code;
}

/** close the server with all connections, stop the loop and free all stuff */
void cAsyncSocketServer::close()
{
	mbRun = false;
	tCLIt it;
	for(it = mConnList.begin(); it != mConnList.end(); it++)
	{
		if (*it) {
			mConnChooser.DelConn(*it);
			if( mFactory!= NULL) mFactory->DeleteConn(*it);
			else delete *it;
			*it = NULL;
		}
	}
	//try{mServerSock.shutdown(sockbuf::shut_readwrite);}catch(...){} // destructor does it itself
}

/** Read property of int mPort. */
const int& cAsyncSocketServer::getmPort(){
	return mPort;
}

/** Write property of int mPort. */
void cAsyncSocketServer::setmPort( const int& _newVal){
	mPort = _newVal;
}

/** add given connection to server, the pointer is managed then by the server,
	it's deleted, when no more use for it*/
void cAsyncSocketServer::addConnection(cAsyncConn *new_conn)
{
	if(!new_conn) throw "addConnection null pointer";
	if(!new_conn->ok)
	{
		if(new_conn->Log(3)) new_conn->LogStream() << "Access refused " << new_conn->AddrIP() << endl;
		new_conn->mxMyFactory->DeleteConn(new_conn);
		return;
	}
	mConnChooser.AddConn(new_conn);

	//cout << "Adding connection on socket: " << new_conn->mSockDesc << endl;

	mConnChooser.cConnChoose::OptIn(
		(cConnBase *)new_conn,
		cConnChoose::tChEvent( cConnChoose::eCC_INPUT|cConnChoose::eCC_ERROR));
	tCLIt it = mConnList.insert(mConnList.begin(),new_conn);
	new_conn->mIterator = it;
	if(0 > OnNewConn(new_conn)) delConnection(new_conn);
}

/** remove given connection from server */
void cAsyncSocketServer::delConnection(cAsyncConn *old_conn)
{
	if(!old_conn) throw "delConnection null pointer";
	if(mNowTreating == old_conn)
	{
		old_conn->ok = false;
		return;
	}
	tCLIt it = old_conn->mIterator;//find(mConnList.begin(), mConnList.end(), old_conn);
	cAsyncConn *found=(*it);
	if( (it ==  mConnList.end()) || (found != old_conn) )
	{
		cout << "not found " << old_conn << endl;
		throw "Trying to delete non-existent connection";
	}

	//cout << "Deleting connection on socket: " << old_conn->mSockDesc << endl;
	mConnChooser.DelConn(old_conn);
	mConnList.erase(it);
	tCLIt emptyit;
	old_conn->mIterator = emptyit;
	
	if (old_conn->mxMyFactory != NULL)
		old_conn->mxMyFactory->DeleteConn(old_conn);
	else delete old_conn;
}

/** perform input operation, read all data from the connection, return number of bytes read, return negative number, if error occured */
int cAsyncSocketServer::input(cAsyncConn *conn)
{
	int just_read=0;
	// read all data available into a buffer
	//try{
		if(conn->ReadAll()<=0) return 0;
	//}catch(...){cout << "Exception in ReadAll" << endl; return 0;}
	while(conn->ok && conn->mWritable)
	{
		// create new line obj if necessary
		if(conn->LineStatus() == AC_LS_NO_LINE)
			conn->SetLineToRead(FactoryString(conn),'|',mMaxLineLength);
		// read data into it from the buffer
		just_read += conn->ReadLineLocal();
		/// test line done
		if(conn->LineStatus() == AC_LS_LINE_DONE)
		{
			OnNewMessage(conn,conn->GetLine());
			conn->ClearLine();
			// connection may be closed after this
		}
		if( conn->BufferEmpty() ) break;
	}
	return just_read;
}

/** perform output operation */
int cAsyncSocketServer::output(cAsyncConn * conn)
{
	conn->Flush();
	return 0;
}

/** treat message for given connection */
void cAsyncSocketServer::OnNewMessage(cAsyncConn *, string *str)
{
	delete str;
}

/** create somehow a string to get line for given connection, ad return th pointer */
string * cAsyncSocketServer::FactoryString(cAsyncConn *)
{
	return new string;
}

/** trigger is called when connection is closed */
void cAsyncSocketServer::OnConnClose(cAsyncConn* conn)
{
	if(!conn) return;
	mConnChooser.DelConn(conn);
}

/** return negative if conn should be removed or error */
int cAsyncSocketServer::OnNewConn(cAsyncConn*conn)
{
	if(!conn) return -1;
	return 0;
}

/** this function is going to be executed periodicaly every N seconds and will call function of the same name in evey connection */
int cAsyncSocketServer::OnTimerBase(cTime &now)
{
	tCLIt it;
	OnTimer(now);
	if( (mT.conn + timer_conn_period) <= now)
	{
		mT.conn = now;
		for(it=mConnList.begin(); it != mConnList.end(); it++)
			if((*it)->ok) (*it)->OnTimerBase(now);
	}
	return 0;
}

/** this is called every period of time */
int cAsyncSocketServer::OnTimer(cTime &now)
{
	return 0;
}

/** do one time step, accept incomming connection, take care of existing, close closed connections */
void cAsyncSocketServer::TimeStep()
{
	cTime tmout(0,1000l);
	//try
	{
		int n = mConnChooser.Choose(tmout);
		if(!n)
		{
			#if ! defined _WIN32
			::usleep(50);
			#else
			::Sleep(0);
			#endif
			return;
		}
	}
	//catch(const char *str){cout << "Exception, after poll: " << str << endl;return;}
	//catch(...){throw "exception in step,poll";}

#if !USE_SELECT
	cConnChoose::iterator it;
#else
	cConnSelect::iterator it;
#endif
	cConnChoose::sChooseRes res;
	for(it = mConnChooser.begin(); it != mConnChooser.end(); )
	{
		res = (*it);
		++it;
		mNowTreating = (cAsyncConn* )res.mConn;
		cAsyncConn *conn = mNowTreating;
		int activity = res.mRevent;
		bool &OK = conn->ok;

		if(!mNowTreating) continue;
		// some connections may have been disabled during this loop, so skip them
		if(OK && (activity & cConnChoose::eCC_INPUT) && conn->GetType() == eCT_LISTEN)
		{
			// accept incomming connection
			int i=0;
			cAsyncConn *new_conn;
			do
			{
				new_conn = conn->Accept();
				if(new_conn) addConnection(new_conn);
				i++;
			} while(new_conn && i <= 101);
#ifdef _WIN32
			cout << "num connections" << mConnChooser.mConnList.size() << endl;
#endif

		}
		if(OK && (activity & cConnChoose::eCC_INPUT)  && 
			((conn->GetType() == eCT_CLIENT) || (conn->GetType() == eCT_CLIENTUDP)))  
			// data to be read or data in buffer
		{
			// returns number of bytes read, on error returns negative code, when connection shell be closed
			//try{
				if(input(conn) <= 0) OK=false;
			//}
			//catch(const char *str){cout << "Exception in input: " << str << endl;}
			//catch(...){throw "exception ass::select/input";}
		}
		if(OK && (activity & cConnChoose::eCC_OUTPUT))
		{
			// NOTE: in sockbuf::write is a bug, missing buf increment, it will block until whole buffer is sent
			//try{
				output(conn);
			//}
			//catch(const char *str){cout << "Exception in step 2: " << str << endl;}
			//catch(...){throw "  cAsyncSocketServer::step 2()";}

		}
		mNowTreating = NULL;
		if(!OK || (activity & (cConnChoose::eCC_ERROR | cConnChoose::eCC_CLOSE)))
		{
			//try{
				delConnection(conn);//}
			//catch(const char*str){cout << "Error in delcon: " << str << endl;}
			//catch(...){throw "ass:step/delconn";}
		}
	}
}

/*!
    \fn cAsyncSocketServer::Listen(int OnPort, bool UDP = false)
 */
cAsyncConn * cAsyncSocketServer::Listen(int OnPort, bool UDP)
{
	cAsyncConn *ListenSock;
	
	if(!UDP) ListenSock = new cAsyncConn(0, this, eCT_LISTEN);
	else	 ListenSock = new cAsyncConn(0, this, eCT_CLIENTUDP);
	
	if (this->ListenWithConn(ListenSock, OnPort, UDP) != NULL)
	{
		return ListenSock;
	}
	else
	{
		delete ListenSock;
		return NULL;
	}
}


/*!
    \fn cAsyncSocketServer::StartListening(int OverrideDefaultPort)
 */
int cAsyncSocketServer::StartListening(int OverrideDefaultPort)
{
	if(OverrideDefaultPort && !mPort) mPort = OverrideDefaultPort;
	if(mPort && !OverrideDefaultPort) OverrideDefaultPort = mPort;
	if(this->Listen(OverrideDefaultPort, false) != NULL) return 0;
	return -1;
}

/*!
    \fn cAsyncSocketServer::ListenWithConn(cAsyncconn *, int OnPort, bool UDP=false)
 */
cAsyncConn * cAsyncSocketServer::ListenWithConn(cAsyncConn *ListenSock, int OnPort, bool UDP)
{
	if (ListenSock != NULL)
	{
		if(ListenSock->ListenOnPort(OnPort,mAddr.c_str(), UDP)< 0) {
			if(Log(0)) LogStream() << "Can't listen on " << mAddr << ":" << OnPort << (UDP?" UDP":" TCP") << endl;
			throw "Can't listen";
			return NULL;
		}
		this->mConnChooser.AddConn(ListenSock);
		this->mConnChooser.cConnChoose::OptIn(
			(cConnBase *)ListenSock,
			cConnChoose::tChEvent( cConnChoose::eCC_INPUT|cConnChoose::eCC_ERROR));
		if(Log(0)) LogStream() << "Listening for connections on " << mAddr << ":" << OnPort << (UDP?" UDP":" TCP") << endl;
		return ListenSock;
	}
	return NULL;
}

bool cAsyncSocketServer::StopListenConn(cAsyncConn *ListenSock)
{
	if (ListenSock != NULL)
	{
		this->mConnChooser.DelConn(ListenSock);
		return true;
	} else return false;
}

};
