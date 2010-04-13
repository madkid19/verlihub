#include <zlib.h>
#include "czlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace nZlib;

cZLib::cZLib()
{
	outBuffer = (char *) calloc(ZLIB_BUFFER_SIZE, 1);
	outBufferLen = ZLIB_BUFFER_SIZE;
	memcpy(outBuffer, "$ZOn|", 5);
}

cZLib::~cZLib()
{
	if(outBuffer)
		free(outBuffer);
}

char *cZLib::Compress(const char *buffer, size_t len, size_t &outLen)
{
	z_stream strm;
	bzero((void *) &strm, sizeof(strm));
	if(outBufferLen < len)
		for(; outBufferLen < len; outBufferLen += ZLIB_BUFFER_SIZE);
	
	outBuffer = (char *) realloc(outBuffer, outBufferLen);
	
	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.data_type = Z_TEXT;
	
	if (deflateInit(&strm, Z_BEST_COMPRESSION) != Z_OK)
		return NULL;
	
	strm.avail_in = (uInt) len;
	strm.next_in  = (Bytef*) buffer;
	
	strm.next_out = (Bytef*) outBuffer+ 5; /** $ZOn| **/
	strm.avail_out = (uInt) (outBufferLen-5);
	
	// compress
	if(deflate(&strm, Z_FINISH) != Z_STREAM_END) {
		deflateEnd(&strm);
		return NULL;
	}
	//TODO: Check if outLen > inputLen => no compression
	
	outLen = strm.total_out + 5; /** $ZOn and pipe **/
	deflateEnd(&strm);
	return outBuffer;
}