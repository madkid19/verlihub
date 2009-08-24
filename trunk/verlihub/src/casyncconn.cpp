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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "casyncsocketserver.h"
#if defined _WIN32
#  include <Winsock2.h>
#  define ECONNRESET WSAECONNRESET
#  define ETIMEDOUT WSAETIMEDOUT
#  define EHOSTUNREACH WSAEHOSTUNREACH
#  define socklen_t int
#  define sockoptval_t char
#endif

#if defined _WIN32 || HAVE_OSTREAM
#include <ostream>
#else
namespace std{
#include <ostream.h>
};
#endif

#if HAVE_ERRNO_H
#include <errno.h>
#endif

#include "casyncconn.h"
#include "cprotocol.h"
#if !defined _WIN32
#  include <arpa/inet.h>
#  include <netinet/in.h>        /* for sockaddr_in */
#  include <sys/socket.h>        /* for AF_INET */
#  include <netdb.h>             /* for gethostbyaddr */
#endif
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "ctime.h"
#include "stringutils.h"

#if ! defined _WIN32
#  define sockoptval_t int
inline int closesocket(int s ){ return ::close(s);}
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

using namespace std;
using namespace nUtils;
using namespace nStringUtils;

namespace nServer {

char *cAsyncConn::msBuffer = new char[MAX_MESS_SIZE+1];
unsigned long cAsyncConn::sSocketCounter = 0;	

cAsyncConn::cAsyncConn(int desc, cAsyncSocketServer *s, tConnType ct):
	cObj("cAsyncConn"),
	mSockDesc(desc),
	mIterator(0),
	ok(desc>0),
	mWritable(true),
	mExtraPoll(0),
	mxServer(s),
	mxMyFactory(NULL),
	mxAcceptingFactory(NULL),
	mxProtocol(NULL),
	mpMsgParser(NULL),
	mAddrPort(0),
	mType(ct)
{
	mMaxBuffer=MAX_SEND_SIZE;
	struct sockaddr saddr;
	struct sockaddr_in *addr_in;
	socklen_t addr_size;

	addr_size = sizeof(saddr);
	unsigned int addr;

	mIp=0;
	ClearLine();
	mBufEnd = mBufReadPos = 0;
	mRegFlag = 0;
	if(mSockDesc)
	{
		if(0 > getpeername(mSockDesc, &saddr, &addr_size))
		{
			if(Log(2)) LogStream() << "Error getting peername, closing" << endl;
			CloseNow();
		}
		addr_in = (struct sockaddr_in *)&saddr;
		// copy IP
		addr = mIp = addr_in->sin_addr.s_addr;
		// asci ip addr
		mAddrIP = inet_ntoa(addr_in->sin_addr);
		// host name
		if( mxServer && mxServer->mUseDNS ) DNSLookup();
		// port number
		mAddrPort=addr_in->sin_port;
	}
	memset (&mCloseAfter,0, sizeof(mCloseAfter));
}


/** connect to given host (ip) on port */
cAsyncConn::cAsyncConn(const string &host , int port, bool udp):
	cObj("cAsyncConn"),
#if !defined _WIN32
	mSockDesc(-1),
#else
	mSockDesc(0),
#endif
	mBufEnd(0),
	mBufReadPos(0),
	mRegFlag(0),
	mCloseAfter(0,0),
	mIterator(0),
	ok(false),
	mWritable(true),
	mExtraPoll(0),
	mxServer(NULL),
	mxMyFactory(NULL),
	mxAcceptingFactory(NULL),
	mxProtocol(NULL),
	mpMsgParser(NULL),
	mAddrPort(port),
	mType(eCT_SERVER)
{
	mMaxBuffer=MAX_SEND_SIZE;
	ClearLine();
	if(udp)
	{
		mType=eCT_SERVERUDP;
		SetupUDP(host, port);
	}
	else
		Connect(host,port);
}

cAsyncConn::~cAsyncConn()
{
	if (mpMsgParser) this->DeleteParser(mpMsgParser);
	mpMsgParser = NULL;
	this->Close();
}

/** close connection to peer */
void cAsyncConn::Close()
{
	if(mSockDesc <= 0) return;
	mWritable=false;
	ok=false;
	if(mxServer) mxServer->OnConnClose(this);
	TEMP_FAILURE_RETRY(closesocket(mSockDesc));
	if(errno != EINTR){
		sSocketCounter --;
		if (Log(3)) LogStream() << "Closing socket " << mSockDesc << endl;
	}		
	else if(ErrLog(1)) LogStream() << "Socket not closed" << endl;
	mSockDesc=0;
}

/** flush as much from output buffer as possible to the iochannel */
void cAsyncConn::Flush()
{
	int size=mBufSend.length();
	string empty("");
	if(size) Write(empty, true); // writes what is in the buffer already
}

/** reads a line from the msBuffer previously filled by ReadAll into private members */
int cAsyncConn::ReadLineLocal()
{
	if(!mxLine) throw "ReadLine with null line pointer";
	char *pos,*buf;
	buf = msBuffer + mBufReadPos;
	int len;
	len = mBufEnd - mBufReadPos;

	if(NULL == (pos = (char*)memchr(buf, mSeparator,len)))
	{
		if(mxLine->size()+len > mLineSizeMax)
		{
			CloseNow();
			return 0;
		}
		mxLine->append((char*)buf,len);
		mBufEnd = 0;
		mBufReadPos = 0;	
		return len;
	}
	len = pos - buf;

	mxLine->append((char*)buf,len);
	mBufReadPos += len+1;
	meLineStatus = AC_LS_LINE_DONE;
	
	return len+1;
}

/** you can provide a string for reading a line by this function
<precond>
LineStatus() == AC_LS_NO_LINE
strp != NULL
</precond> */
void cAsyncConn::SetLineToRead(string *strp,char delim, int max)
{
	if(LineStatus() != AC_LS_NO_LINE) throw "cAsyncConn::SetLineToRead - precondition not ok";
	if(!strp) throw "cAsyncConn::SetLineToRead - precondition not ok - null string pointer";
	meLineStatus = AC_LS_PARTLY;
	mLineSize = 0;
	mLineSizeMax = max;
	mxLine = strp;
	mSeparator = delim;
}

/** clears the line status */
void cAsyncConn::ClearLine()
{
	meLineStatus = AC_LS_NO_LINE;
	mLineSize = 0;
	mLineSizeMax = 0;
	mSeparator = '\n';
	mxLine = NULL;
}


/** return the line's pointer and keep it */
string * cAsyncConn::GetLine()
{
	return mxLine;
}

/** No descriptions */
void cAsyncConn::CloseNice(int msec)
{
	OnCloseNice();
	mWritable = false;
	if(msec <= 0) { CloseNow(); return;}
	if(!mBufSend.size()) { CloseNow(); return;}
	
	mCloseAfter.Get();
	mCloseAfter += int(msec);
}

/** immediately close the connection */
void cAsyncConn::CloseNow()
{
	mWritable = false;
	ok = false;
	if(mxServer) mxServer->mConnChooser.cConnChoose::OptOut((cConnBase*)this, cConnChoose::eCC_ALL);
	if(mxServer) mxServer->mConnChooser.cConnChoose::OptIn((cConnBase*)this, cConnChoose::eCC_CLOSE);
}


/** reads all available data from the socket and stores it into a static member buffer */
int cAsyncConn::ReadAll()
{
	int buf_len = 0 , i=0, addr_len = sizeof(struct sockaddr);
	mBufReadPos = 0;
	mBufEnd = 0;
	bool udp = (this->GetType() == eCT_CLIENTUDP);
	
	if(!ok || !mWritable) return -1;

	if(!udp)
	{
		while(
			((buf_len = recv(mSockDesc, msBuffer, MAX_MESS_SIZE, 0)) == -1) &&
			((errno == EAGAIN) || (errno == EINTR))
			&& (i++ <= 100)
		)	{
#if ! defined _WIN32
	    ::usleep(5);
#endif
		}
	}
	else
	{
		while(
			((buf_len = recvfrom(mSockDesc, msBuffer, MAX_MESS_SIZE, 0, (struct sockaddr *)&mAddrIN, (socklen_t *)&addr_len)) == -1) &&
			(i++ <= 100)
		) {
#if ! defined _WIN32
			::usleep(5);
#endif
		}
	}

	if(buf_len <= 0)
	{
		if(!udp)
		{
			if(buf_len == 0)
			{
				/* Connection closed - hung up*/
				if(Log(2)) LogStream() << "User hung up.." << endl;
				CloseNow();
				return -1;
			}
			else
			{
				if(Log(2)) LogStream() << "Read IO Error: " << errno << " : " << strerror(errno) << endl;
				switch(errno)
				{
					case ECONNRESET:/* connection reset by peer */ break;
					case ETIMEDOUT:/* connection timed out */break;
					case EHOSTUNREACH:/* No route to host */ break;
					default:break;
				} // end switch
			}
			CloseNow();
			return -1;
		}
	}
	else
	{
		// received data
		mBufEnd = buf_len;
		mBufReadPos = 0;
		msBuffer[mBufEnd]='\0'; // end the string
		mTimeLastIOAction.Get();
	}
	return buf_len;
}

/* Sends as many packets as it takes. */
/* This was taken from Beej's guide to network programming: */
/* http://www.ecst.csuchico.edu/~beej/guide/net/html/ */
int cAsyncConn::SendAll(const char *buf, size_t &len)
{
	size_t total = 0;        /* how many bytes we've sent */
	size_t bytesleft = len; /* how many we have left to send */
	int n = 0;
	int repetitions=0;
	bool udp = (this->GetType() == eCT_SERVERUDP);

#ifndef QUICK_SEND
	while(total < len)
	{
		try
		{
			if(!udp)
			{
#if ! defined _WIN32
				n = send(mSockDesc, buf + total, bytesleft, MSG_NOSIGNAL|MSG_DONTWAIT);
#else
				int RetryCount = 0;
				do
				{ 
					if ((n = send(mSockDesc, buf + total, (int)bytesleft, 0)) != SOCKET_ERROR)
						break;
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
					  if(ErrLog(3)) LogStream() << "cAsynConn Warning.. Resource unavailable...Retrying.. " << ++RetryCount <<endl;
							::Sleep(50);
					}
				}
				while (WSAGetLastError() == WSAEWOULDBLOCK);
#endif
			}
			else
			{
				n = sendto(mSockDesc, buf + total, bytesleft, 0, (struct sockaddr *)&mAddrIN, sizeof(struct sockaddr));
			}
    		}
		catch(...)
		{
			if(ErrLog(2))
				LogStream() << "exception in SendAll(buf," << len
					<< ") total=" << total
					<< " left=" << bytesleft
					<< " rep=" << repetitions
					<< " n=" << n << endl;
			return -1;
		}
		repetitions++;
		if(n == -1)
			break;
		total += n;
		bytesleft -= n;
	}
#else
	if(!udp) {
		n = send(mSockDesc, buf + total, bytesleft, 0);
	}
	else
		n = sendto(mSockDesc, buf + total, bytesleft, 0, (struct sockaddr *)&mAddrIN, sizeof(struct sockaddr));
	total = n;
#endif
	len = total; /* return number actually sent here */
	return n == -1?-1:0; /* return -1 on failure, 0 on success */
}

/** Setup the destination address for UDP */
int cAsyncConn::SetupUDP(const string &host, int port)
{
	mSockDesc = CreateSock(true);

	if(mSockDesc == INVALID_SOCKET)
	{
		cout << "Error getting socket.\n" << endl;
		ok = false;
		return -1;
	}

	struct hostent *he = gethostbyname(host.c_str());
	if(he != NULL)
	{
		memset(&mAddrIN, 0, sizeof(struct sockaddr_in));
		mAddrIN.sin_family = AF_INET;
		mAddrIN.sin_port = htons(port);
		mAddrIN.sin_addr = *((struct in_addr *)he->h_addr);
		memset(&(mAddrIN.sin_zero), '\0', 8);
		ok = true;
		return 0;
	}
	else
	{
		cout << "Error resolving host " << host << endl;
		ok=false;
		return -1;
	}
}

int cAsyncConn::SendUDPMsg(const string &host, int port, const string &data)
{
	int _result;
	cAsyncConn conn(host, port, true);
	if (conn.ok) _result =conn.Write(data, true);
	else return -1;
	if(conn.mSockDesc != INVALID_SOCKET) conn.Close();
	return _result;
}

/** connect to given host (ip) on port */
int cAsyncConn::Connect(const string &host, int port)
{
	struct sockaddr_in dest_addr;
	mSockDesc = CreateSock();

	if(mSockDesc == INVALID_SOCKET)
	{
		cout << "Error getting socket.\n" << endl;
		ok = false;
		return -1;
	}
	cTime timeout(5.0);
	SetSockOpt(SO_RCVTIMEO, &timeout, sizeof(timeval));
	SetSockOpt(SO_SNDTIMEO, &timeout, sizeof(timeval));

	struct hostent *he = gethostbyname(host.c_str());
	if(he != NULL)
	{
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(port);
		dest_addr.sin_addr.s_addr = *(unsigned*)(he->h_addr_list[0]);//inet_addr(host.c_str());
		memset(&(dest_addr.sin_zero), '\0', 8);

		int s = connect(mSockDesc, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
		if(s==-1)
		{
			cout << "Error connecting to " << host << ":" << port << endl;
			ok=false;
			return -1;
		}
		ok=true;
		return 0;
	}
	else
	{
		cout << "Error resolving host " << host << endl;
		ok=false;
		return -1;
	}
}

int cAsyncConn::SetSockOpt(int optname, const void *optval, int optlen)
{
#ifndef WIN32
   return setsockopt(this->mSockDesc, SOL_SOCKET, optname, optval , optlen);
#else
   //return 0;
#endif
}

int cAsyncConn::GetSockOpt(int optname,       void *optval, int &optlen)
{
	int result=0;
#ifndef WIN32
	socklen_t _optlen;
	result = getsockopt(this->mSockDesc, SOL_SOCKET, optname, optval , &_optlen);
#endif
	//optlen = optlen;
	return result;
}

/** create a socket */
tSocket cAsyncConn::CreateSock(bool udp)
{
	tSocket sock;
	sockoptval_t yes=1;
	
	if(!udp)
	{
		/* Create tcp socket */
		if((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			return INVALID_SOCKET;

		/* Fix the address already in use error */
		if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(sockoptval_t)) == INVALID_SOCKET)
			return INVALID_SOCKET;
	}
	else
	{
		/* Create udp socket */
		if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
			return INVALID_SOCKET;
	}

	sSocketCounter ++;
	if (Log(3)) LogStream() << "New socket " << sock << endl;

	return sock;
}

/** binds given socket to a port */
int cAsyncConn::BindSocket(int sock, int port, const char *ia)
{
	if(sock <0) return -1;
	memset(&mAddrIN, 0, sizeof(struct sockaddr_in));
	mAddrIN.sin_family = AF_INET;
	mAddrIN.sin_addr.s_addr = INADDR_ANY; // default listen address
	if(ia) 
#if !defined _WIN32
	inet_aton(ia, &mAddrIN.sin_addr); // override it
#else
	mAddrIN.sin_addr.s_addr = inet_addr(ia);
#endif
	mAddrIN.sin_port = htons(port);
	memset(&(mAddrIN.sin_zero), '\0', 8);


	/* Bind socket to port */
	if(bind(sock, (struct sockaddr *)&mAddrIN, sizeof(mAddrIN)) == -1)
	{
		return -1;
	}
	return sock;
}

/** listen on given socket */
int cAsyncConn::ListenSock(int sock)
{
	if(sock <0) return -1;
	if(listen(sock, 100) == -1)
	{
		cout << "Error listening" << endl;
		return -1;
	}
	return sock;
}

/** set socket for non-blocking */
tSocket cAsyncConn::NonBlockSock(int sock)
{
	if(sock<0) return -1;
#if !defined _WIN32
	int flags;
	if((flags = fcntl(sock, F_GETFL, 0)) < 0) return INVALID_SOCKET;
	if( fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0 ) return INVALID_SOCKET;
#else
	unsigned long one=1;
	if(SOCKET_ERROR == ioctlsocket(sock, FIONBIO, &one)) return INVALID_SOCKET;
#endif
	return sock;
}

/** No descriptions */
int cAsyncConn::ListenOnPort(int port, const char *ia, bool udp)
{
	if(mSockDesc) return -1;
	mSockDesc=CreateSock(udp);
	mSockDesc=BindSocket(mSockDesc,port,ia);
	if(!udp)
	{
	    mSockDesc=ListenSock(mSockDesc);
	    mSockDesc=NonBlockSock(mSockDesc);
	}
	ok = mSockDesc > 0;
	return mSockDesc;
}

/** return new socket */
tSocket cAsyncConn::AcceptSock()
{
	socklen_t namelen;
	tSocket socknum = INVALID_SOCKET;
	sockoptval_t yes = 1;
	int i=0;
	#if ! defined _WIN32
	struct sockaddr_in client;
	#else
	struct sockaddr client;
	#endif


	/* Get a socket for the connected user.  */
	namelen = sizeof(client);
	memset(&client, 0, namelen);

	#if ! defined _WIN32
	socknum = ::accept(mSockDesc, (struct sockaddr *)&client, &namelen);
	#else
	socknum = accept(mSockDesc, (struct sockaddr *)&client, &namelen);
	#endif
	while(( socknum == INVALID_SOCKET) && ((errno == EAGAIN) || (errno == EINTR)) && (i++ < 10))
	{
		#if ! defined _WIN32
		socknum = ::accept(mSockDesc, (struct sockaddr *)&client, (socklen_t*)&namelen);
		#else
   	socknum = accept(mSockDesc, (struct sockaddr *)&client, &namelen);
		#endif
		
		#if ! defined _WIN32
			::usleep(50);
		#else
         ::Sleep(1);
		#endif
	}
	if(socknum == INVALID_SOCKET) 
	{
		#ifdef _WIN32
		cout << WSAGetLastError() << "  " << sizeof(fd_set) << endl;
		#endif
		return INVALID_SOCKET;
	}
	if (Log(3)) LogStream() << "Accepted Socket " << socknum << endl;
	sSocketCounter++;

#ifndef _WIN32
	if(setsockopt(socknum, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == SOCKET_ERROR )
	{
		TEMP_FAILURE_RETRY(closesocket(socknum));
		if(errno != EINTR) {
			sSocketCounter --;
			if (Log(3)) LogStream() << "Closing socket " << socknum << endl;
		}
		else if(ErrLog(1)) LogStream() << "Socket not closed" << endl;
		return INVALID_SOCKET;
	}
#endif
	if((socknum = NonBlockSock(socknum)) == INVALID_SOCKET) return INVALID_SOCKET;

	return socknum;
}

cConnFactory * cAsyncConn::GetAcceptingFactory()
{
	if (this->mxAcceptingFactory) return mxAcceptingFactory;
	if( mxServer && mxServer->mFactory) return mxServer->mFactory;
	return NULL;
}

/** create new connection from the listening port */
cAsyncConn * cAsyncConn::Accept()
{
	tSocket sd;
	cConnFactory *AcceptingFactory = NULL;
	cAsyncConn *new_conn = NULL;

	sd = AcceptSock();
	if(sd == INVALID_SOCKET ) return NULL;
	mTimeLastIOAction.Get();
	
	AcceptingFactory = this->GetAcceptingFactory();
	if (AcceptingFactory != NULL)
		new_conn = AcceptingFactory->CreateConn(sd);
	if(!new_conn) throw "can't create connection";

	return new_conn;
}

/** Read property of tConnType mType. */
const tConnType& cAsyncConn::getType() { return mType; }

/** this function is going to be executed periodicaly every N seconds by the  function of the same name in server */
int cAsyncConn::OnTimerBase(cTime &now)
{

	if( bool(mCloseAfter) && (mCloseAfter > now) )
	{
		CloseNow();
		return 0;
	}
	Flush();
	OnTimer(now);
	return 0;
}

/** this is called every period of time */
int cAsyncConn::OnTimer(cTime &now){ return 0;}

/** this is called when write buffer gets empty */
void cAsyncConn::OnFlushDone(){}

/** write given data or store into buffer */
int cAsyncConn::Write(const string &data, bool Flush)
{
	static string tmp;

	// Append data to older data in buffer
	// but only if something is there and if free space
	// 1st) check the size
	if(mBufSend.size()+ data.size() >= mMaxBuffer)
	{
		if(Log(2)) LogStream() << "Buffer is too big, closing" << endl;
		CloseNow();
		return -1;
	}

	Flush = Flush || (mBufSend.size() > (mMaxBuffer >> 1));
	
	// a buffer from which to send data
	const char *send_buffer;
	// size of data to send
	size_t send_size;
	// indicator from which we rae sending
	bool appended;

	// second decide whether append or not
	if(mBufSend.size() || !Flush)
	{
		mBufSend.append(data.data(), data.size());
		send_buffer= mBufSend.data();
		send_size  = mBufSend.size();
		appended   = true;
	}
	else
	{
		send_buffer= data.data();
		send_size  = data.size();
		appended   = false;
	}

	// if there isn't anything to send, don't bother
	if(!send_size) return 0;

	if (!Flush) return 0;
	
	// make copy of send_size, size_sent will be modified by next function
	size_t size_sent = send_size;
	// now send the data, as much as possible
	if(SendAll(send_buffer,size_sent) == -1)
	{
		// analyze the error
		if((errno != EAGAIN) && (errno != EINTR))
		{
			// error during writing, remove the user
			if(Log(2)) LogStream() << "Error during writing, closing" << endl;
			CloseNow();
			return -1;
		}

		// if something has been sent, but not all, update the buffer
		if(size_sent > 0)
		{
			mTimeLastIOAction.Get();
			// now it depends on if we appended or not
			// * if we appended, assign the rest of string to the buffer
			if(!appended)
				// this is supposed to actually reduce the size of mBufSend
				// it does a copy so it is slower but memory usage is important thing
				StrCutLeft(data, mBufSend, size_sent);
			else
				// this makes mBuffSend not grow
				StrCutLeft( mBufSend, size_sent);
		}
		else
		{
			// close nice was called, we must close the connection even if the data cannot be transmitted.
			if(bool(mCloseAfter)) CloseNow();
		}

		// Buffer overfill protection - only on registered connections
		if(mxServer && ok)
		{
			// be chosen next time to send the rest of it
			mxServer->mConnChooser.cConnChoose::OptIn(this, cConnChoose::eCC_OUTPUT);

			if(mBufSend.size() < MAX_SEND_UNBLOCK_SIZE )
			{
				mxServer->mConnChooser.cConnChoose::OptIn(this, cConnChoose::eCC_INPUT);
				if(Log(5)) LogStream() << "UnBlock INPUT" << endl;
			}
			else if( mBufSend.size() >= MAX_SEND_FILL_SIZE )
			{
				mxServer->mConnChooser.cConnChoose::OptOut(this, cConnChoose::eCC_INPUT);
				if(Log(5)) LogStream() << "Block INPUT" << endl;
			}
		}
	}
	else // everything sent all right
	{
		// on success clear output buffer
		if(appended) mBufSend.erase(0, mBufSend.size());
		ShrinkStringToFit(mBufSend);

		// close nice was called, so we close now
		if(bool(mCloseAfter)) CloseNow();

		// unregister writing
		if(mxServer && ok)
		{
			mxServer->mConnChooser.cConnChoose::OptOut(this, cConnChoose::eCC_OUTPUT);
			if(Log(5)) LogStream() << "Blocking OUTPUT " << endl;
		}

		mTimeLastIOAction.Get();
		// report that is done
		OnFlushDone();
	}
	return size_sent;
}

/** function called before closing nicely */
int cAsyncConn::OnCloseNice(void) { return 0; }

cMessageParser *cAsyncConn::CreateParser()
{
	if (this->mxProtocol != NULL)
		return this->mxProtocol->CreateParser();
	else return NULL;
}

void cAsyncConn::DeleteParser(cMessageParser *OldParser)
{
	if (this->mxProtocol != NULL)
		this->mxProtocol->DeleteParser(OldParser);
	else delete OldParser;
}

/** create a string to get a line for a given connection and return the pointer */
string * cAsyncConn::FactoryString()
{
	if (mpMsgParser == NULL) mpMsgParser = this->CreateParser();
	if (mpMsgParser == NULL) return NULL;
	mpMsgParser->ReInit();
	return &(mpMsgParser->GetStr());
}


};


/*!
    \fn nServer::cAsyncConn::DNSLookup()
 */
//*NOTE:DNSLookup is extremely heavy on resources when dealing with a large amount of connections.
bool nServer::cAsyncConn::DNSLookup()
{
	struct hostent *hp;
	if(mAddrHost.size()) return true;
	if(hp=gethostbyaddr((char *)&mIp,sizeof(mIp),AF_INET))
		mAddrHost=hp->h_name;
	return hp != NULL;
}

unsigned long nServer::cAsyncConn::DNSResolveHost(const string &host)
{	
	struct sockaddr_in AddrIN;
	memset(&AddrIN, 0, sizeof(sockaddr_in));
	struct hostent *he = gethostbyname(host.c_str());
	if(he != NULL)
	{
		AddrIN.sin_addr = *((struct in_addr *)he->h_addr);
	}
	return AddrIN.sin_addr.s_addr;
}

bool nServer::cAsyncConn::DNSResolveReverse(const string &ip, string &host)
{
	struct hostent *hp;
	struct in_addr addr;
#ifndef _WIN32
	if(!inet_aton(ip.c_str(), &addr)) return false;
#else
	addr.s_addr = inet_addr(ip.c_str());
#endif
	if(hp = gethostbyaddr((char *)&addr,sizeof(addr),AF_INET))
		host=hp->h_name;
	return hp != NULL;
}

const char *nServer::cAsyncConn::IPAsString(unsigned long addr)
{
	struct in_addr in;
	*((unsigned long*)&in.s_addr)=addr;
	return inet_ntoa(in);
}

/**************************** 
          cConnFactory 
*****************************/
nServer::cAsyncConn * nServer::cConnFactory::CreateConn(tSocket sd)
{
	cAsyncConn *conn = new cAsyncConn(sd);
	conn->mxMyFactory = this;
	return conn;
}

void nServer::cConnFactory::DeleteConn(cAsyncConn * &conn)
{
	conn->Close();
	delete conn;
	conn = NULL;
}


