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
#include <zlib.h>
#include "czlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace nVerliHub {
	namespace nUtils {

cZLib::cZLib() :
zBufferPos(0),
zBufferLen(ZLIB_BUFFER_SIZE),
outBufferLen(ZLIB_BUFFER_SIZE)
{
	outBuffer = (char *) calloc(ZLIB_BUFFER_SIZE, 1);
	zBuffer = (char *) calloc(ZLIB_BUFFER_SIZE, 1);
	memcpy(outBuffer, "$ZOn|", 5);
}

cZLib::~cZLib()
{
	if(outBuffer)
		free(outBuffer);
	if(zBuffer)
		free(zBuffer);
}

char *cZLib::Compress(const char *buffer, size_t len, size_t &outLen)
{
	z_stream strm;
	memset((void *) &strm, '\0', sizeof(strm));
	if((zBufferLen - zBufferPos) < len)
		for(; (zBufferLen - zBufferPos) < len; zBufferLen += ZLIB_BUFFER_SIZE);

	char * new_buffer = (char *) realloc(zBuffer, zBufferLen);
	// TODO: Throw exception and log error
	if (new_buffer == NULL) {
		free(zBuffer);
		return NULL;
	}
	zBuffer = new_buffer;

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.data_type = Z_TEXT;

	if (deflateInit(&strm, Z_BEST_COMPRESSION) != Z_OK)
		return NULL;

	// Copy data in ZLib buffer
	memcpy(zBuffer + zBufferPos, buffer, len);
	zBufferPos += len;

	// Increase out buffer if not enough
	if(outBufferLen < zBufferPos)
		for(; outBufferLen < zBufferPos; outBufferLen += ZLIB_BUFFER_SIZE);

	strm.avail_in = (uInt) zBufferPos;
	strm.next_in  = (Bytef*) zBuffer;

	strm.next_out = (Bytef*) outBuffer + ZON_LEN; /** $ZOn| **/
	strm.avail_out = (uInt) (outBufferLen - ZON_LEN);

	// Compress
	if(deflate(&strm, Z_FINISH) != Z_STREAM_END) {
		deflateEnd(&strm);
		return NULL;
	}

	outLen = strm.total_out + 5; /** $ZOn and pipe **/
	deflateEnd(&strm);
	// If compressed data is bigger than raw data, fallback to raw data
	if(zBufferPos < outLen) {

		outLen = zBufferPos;
		zBufferPos = 0;
		return zBuffer;
	}

	//Clear for DEBUG
	//zBuffer[zBufferPos] = '\0';
	zBufferPos = 0;
	return outBuffer;
}

void cZLib::AppendData(const char *buffer, size_t len)
{
	// Increase ZLib buffer if not enough
	if((zBufferLen - zBufferPos) < len)
		for(; (zBufferLen - zBufferPos) < len; zBufferLen += ZLIB_BUFFER_SIZE);

	char * new_buffer = (char *) realloc(zBuffer, zBufferLen);
	// TODO: Throw exception and log error
	if (new_buffer == NULL) {
		free(zBuffer);
		return;
	}
	zBuffer = new_buffer;
	memcpy(zBuffer + zBufferPos, buffer, len);
	zBufferPos += len;
}
	}; // namespace nUtils
}; // namespace nVerliHub
