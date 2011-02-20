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
#ifndef CASYNCCONN_H
#define CASYNCCONN_H

//Buffer
#define MAX_MESS_SIZE 0x2FFFF           // max length of message that we accept
#define MAX_SEND_SIZE 0x1FFFFE         // max size of output buffer
#define MAX_SEND_FILL_SIZE 0x15FFFE    // on this level, incoming data is blocked
#define MAX_SEND_UNBLOCK_SIZE 0x07FFFFE // under this level it's unblocked again
#include "cobj.h"
#include "ctime.h"
#include "cconnbase.h"
#ifndef WIN32
#include <netinet/in.h>
#endif
#include <string>
#include <list>

namespace nServer {

class cAsyncSocketServer;
class cAsyncConn;
class cProtocol;

class cConnFactory
{
public:
	cConnFactory(cProtocol *Protocol): mProtocol(Protocol) {};
	virtual ~cConnFactory(){};
	virtual cAsyncConn * CreateConn(tSocket sd=0);
	virtual void DeleteConn(cAsyncConn * &);
	cProtocol *mProtocol;
};

namespace nEnums
{
  
// Type of socket connection
enum tConnType
{
	eCT_LISTEN,    // Listening tcp connection
	eCT_CLIENT,    // TCP client connection that is connected to the server
	eCT_CLIENTUDP, // UDP cleint connection
	eCT_SERVER,    // TCP server connection
	eCT_SERVERUDP  // UDP server connection
};

enum
{
	AC_LS_NO_LINE,
	AC_LS_PARTLY,
	AC_LS_LINE_DONE,
	AC_LS_ERROR
};
};

using namespace ::nServer::nEnums;
using namespace std;
using namespace nUtils;

class cMessageParser;

/**
 * Network connection class for asynchronous (aka non-blocking) connection.
 * @author Daniel Muller
 * @author Janos Horvath (UDP support)
 */

class cAsyncConn : public cConnBase, public cObj
{
 public:
	cAsyncConn(int sd=0, cAsyncSocketServer *s=NULL, tConnType ct= eCT_CLIENT);
	cAsyncConn(const string & host, int port, bool udp=false);
	virtual ~cAsyncConn();
	
	/**
	* Close the connection.
	*/
	void Close();
	
	/**
	* Flush and send output buffer data as much as possible to I/O channel.
	*/
	void Flush();
	
	// Socket descriptor
	tSocket mSockDesc;
	
 private:
	//Pointer to read line string
	string *mxLine;
	
	// Line status (partily read, no line, etc.)
	int meLineStatus;
	
	// End buffer position
	int mBufEnd;
	
	// Read buffer position
	int mBufReadPos;
	
	// Registered flags
	int mRegFlag;
	
	// Time when to close the connection
	cTime mCloseAfter;

	// Connection list
	typedef list<cAsyncConn*> tConnList;
	
	// Connection iterator
	typedef tConnList::iterator tCLIt;
	
 public:
	/**
	* Reset the status of the line and delimiter to default value (new line).
	*/
	void ClearLine();
	
	/**
	* Close the connection after given milliseconds.
	* @param msec Millisecond
	*/
	void CloseNice(int msec=0);
	
	/**
	* Return the pointer to the line.
	* @return Pointer string
	*/
	string * GetLine();
	
	/**
	* Return the status of the line.
	* @return Line status
	*/
	int LineStatus() { return meLineStatus; }
	
	/**
	* Set a pointer where to store the line to read and the delimiter.
	* @param strp Pointer to allocated memory area where to store read line (default delimiter is new line).
	* @param delimiter Delimiter for lines
	* @param max Max length of line
	*/
	void SetLineToRead(string *,char , int max=-1);
	

	/**
	 * Read a line and store it to internal buffer. Use GetLine() to return the content of the buffer.
	 * @return Number of read bytes
	 */
	int ReadLineLocal();
	
	// Connection iterator
	tCLIt mIterator;
	
	// Indicate if the server should take care of this connection or not
	bool ok;
	
	// Indicate if data is allowed to be stored in the out buffer
	bool mWritable;
	
	/** flags externall to poll, but used by poller in a way */
	int mExtraPoll;
public:
	// Pointer to server class
	cAsyncSocketServer * mxServer;
	cConnFactory * mxMyFactory;
	cConnFactory * mxAcceptingFactory;
	cProtocol * mxProtocol;
	cMessageParser *mpMsgParser;
	static unsigned long sSocketCounter;	
protected: // Protected methods
	/** static buffer as intermediate place to stock data when reading parts of lines */
	static char *msBuffer;
	/** send buffer, what is not sent immediately, is sent later from this buffer */
	string mBufSend;
	/** line spearator */
	char mSeparator;
	/** byte count already in the line */
	int mLineSize;
	/** ip address */
	string mAddrHost;
	string mAddrIP;
	int mAddrPort;
	unsigned long mMaxBuffer;
	/** byte count limit for the line */
	unsigned mLineSizeMax;
	/** the ip address - binary format */
	unsigned long mIp;
	/** connestion type */
	tConnType mType;
	
	/**
	 * Send len bytes in the buffer.
	 * See http://www.ecst.csuchico.edu/~beej/guide/net/html/.
	 * @param buf Buffer to send
	 * @param len Number of bytes to send
	 * @return Number of sent bytes
	 */
	int SendAll(const char *buf, size_t &len);
	
	/** create a tcp or udp socket (default tcp) */
	tSocket CreateSock(bool udp=false);
	/** binds given socket to a port */
	int BindSocket(int sock, int port, const char *addr=NULL);
	/** listen on given socket */
	int ListenSock(int sock);
	/** set socket for non-blocking */
	tSocket NonBlockSock(int sock);
	/** return new socket */
	tSocket AcceptSock();
	/** function called before closing nicely */
	virtual int OnCloseNice(void);
	/** for udp */
	struct sockaddr_in mAddrIN;
	
public:
	virtual cMessageParser *CreateParser();
	virtual void DeleteParser(cMessageParser *);
	virtual string * FactoryString();
	
	/** retrun IP address */
	const string & AddrIP(){return mAddrIP;}
	const string & AddrHost(){return mAddrHost;}
	const int AddrPort(){return mAddrPort;}
	const unsigned long GetSockAddress() { return mAddrIN.sin_addr.s_addr; }
	
	/**
	 * Read all available data from the socket and store it into a static member buffer
	 * @return Number of read bytes
	 */
	int ReadAll();
	
	/** tells if msBuffer has some more data available or not
	return value: true if not */
	int BufferEmpty(){ return mBufEnd == mBufReadPos;};
	/** No descriptions */
	int ListenOnPort(int port, const char *ia=NULL, bool udp=false);
	/** create new connection from the listening port */
	virtual cAsyncConn * Accept();
	/** Read property of tConnType mType. */
	virtual const tConnType& getType();
	/** this is called every period of time */
	virtual int OnTimer(cTime &now);
	/** this function is going to be executed periodicaly every N seconds by the  function of the same name in server */
	int OnTimerBase(cTime &now);
	
	/**
	 * Immediatly close the connection. Use CloseNice(int msec) not to close it immediately.
	 */
	void CloseNow();
	/** this is called when write buffer gets empty */
	virtual void OnFlushDone();
	/** write given data or store into buffer */
	int Write(const string &data, bool Flush);
	/** register the connection into given selector */
	//void UnRegister(cPoller & );
	/** register the connection into given selector */
	//int Register(cPoller & , int );
	/** No descriptions */
	tConnType GetType(){return mType;};
	/** connect to given host (ip) on port */
	int Connect(const string &, int);
	
	/**
	 * Setup UDP socket.
	 * @param host The hostname
	 * @param port The port
	 * @return Zero on success or -1 on failure
	 */
	int SetupUDP(const string &, int);
	bool DNSLookup();
	bool DNSResolveReverse(const string &ip, string &host);
	virtual operator tSocket()const {return mSockDesc;}
	cTime mTimeLastIOAction;
	// for UDP messages to given host and port
	static int SendUDPMsg(const string &host, int port, const string &data);
	static unsigned long DNSResolveHost(const string &host);
	static string IPAsString(unsigned long addr);

	int SetSockOpt(int optname, const void *optval, int optlen);
	int GetSockOpt(int optname,       void *optval, int &optlen);
protected:
	virtual cConnFactory * GetAcceptingFactory();
};

};

#endif
