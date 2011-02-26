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
#ifndef NSERVERCPROTOCOL_H
#define NSERVERCPROTOCOL_H
#include <vector>
#include "cobj.h"

using namespace std;

namespace nServer
{
	enum { eMSG_UNPARSED=-1 };
	class cMessageParser;
	class cAsyncConn;
	
/**
aBase class for protocols
 
@author Daniel Muller
*/
class cProtocol : public cObj
{
public:
	cProtocol();
	virtual ~cProtocol();
	virtual int TreatMsg(cMessageParser * msg, cAsyncConn *conn) = 0;
	virtual cMessageParser *CreateParser() = 0;
	virtual void DeleteParser(cMessageParser *) = 0;	
};

class cMessageParser : public cObj
{
public:
	cMessageParser(int);
	virtual ~cMessageParser();

	/** parses the string and sets the state variables */
	virtual int Parse() = 0; // must override
	/** splits message to it's important parts and stores their info in the chunkset mChunks */
	virtual bool SplitChunks() = 0; // must override
	
	/** reinitialize the structure */
	virtual void ReInit();
	/** return the n'th chunk (as splited by SplitChunks) function */
	virtual string &ChunkString(unsigned int n);
	
	/** apply the chunkstring ito the main string */
	void ApplyChunk(unsigned int n);
	/** get string */
	string & GetStr();

	
public: // Public attributes
	/** The actual message string */
	string mStr;

	/** the type for message chunks */
	typedef pair<int,int> tChunk;
	typedef vector<tChunk> tChunkList;
	typedef tChunkList::iterator tCLIt;
	typedef vector<string *> tStrPtrList;
	typedef tStrPtrList::iterator tSPLIt;
	/** the list of chunks */
	tChunkList mChunks;
	/** list of string pointers */
	//tStrPtrList mChStrings;
	string *mChStrings;
	/** a bitmap having information about chStringsSet */
	unsigned long mChStrMap;
	
	/** indicates if the message is tooo long so it can't be receved complete */
	bool Overfill;
	/** indicates if the message is completely received */
	bool Received;
	/** error in message indicator */
	bool mError;
	/** parsed message type */
	int mType;
	/** length of the message */
	unsigned mLen;
	unsigned mKWSize;
	
protected:
	/** reduce the chunk from left by amount, cn is the chunk number */
	bool ChunkRedLeft(int cn, int amount);
	/** splits message into two chunks by a delimiter adn stores them in the chunklist */
	bool SplitOnTwo(size_t start, const string & lim, int cn1, int cn2, size_t len=0,bool left=true);
	/** splits the chunk number "ch" into two chunks by a delimiter adn stores them in the chunklist under numbers cn1 and cn2 */
	bool SplitOnTwo(const string & lim, int ch, int cn1, int cn2, bool left=true);
	/** splits message into two chunks by a delimiter adn stores them in the chunklist */
	bool SplitOnTwo(size_t start, const char lim, int cn1, int cn2, size_t len=0,bool left=true);
	/** splits the chunk number "ch" into two chunks by a delimiter adn stores them in the chunklist under numbers cn1 and cn2 */
	bool SplitOnTwo(const char lim, int ch, int cn1, int cn2, bool left=true);
	/** reduce the chunk from right by amount, cn is the chunk number */
	bool ChunkRedRight(int cn, int amount);
	/** fill in a given chunk */
	void SetChunk(int,int,int);

protected: // Private methods
	int mMaxChunks;
	
};

};

#endif
