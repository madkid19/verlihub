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
#ifndef NZLIB_H
#define NZLIB_H

#include <cstring>
#include <string>
#include <stdint.h>

#define ZLIB_BUFFER_SIZE 256*1024 /** deflateInit() will allocate on the order of 256K bytes for the internal state **/

using namespace std;

namespace nZlib {

class cZLib 
{
	public:
		cZLib();
		~cZLib();
		
		void AppendData(const char *buffer, size_t len);
		char *Compress(const char *buffer, size_t len, size_t &outLen);
		//char *GetOutput(size_t &len) { len = outBufferLen; return outBuffer; };
		
	private:
		// Compressed data
		char *outBuffer;
		// Current position in the buffer
		size_t bufferPos;
		// Total buffer length
		size_t outBufferLen;
};


};

#endif
