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
#include "cpcre.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "stringutils.h"

using namespace std;
namespace nVerliHub {
	namespace nUtils {

cPCRE::cPCRE(int coord) :
	mCoords(NULL),
	mCoordsCount(coord)
{
	Clear();
}

cPCRE::~cPCRE(){
	if(mCoords) delete [] mCoords;
	mCoords = NULL;
}

cPCRE::cPCRE(const char *pat, unsigned int options, int coord) :
	mCoords(NULL),
	mCoordsCount(coord)
{
	Clear();
	Compile(pat, options);
}

cPCRE::cPCRE(const string &str, unsigned int options, int coord):
	mCoords(NULL),
	mCoordsCount(coord)
{
	Clear();
	Compile(str.c_str(), options);
}

bool cPCRE::Compile(const char *pat, unsigned int options)
{
	char *errptr;
	int erroffset;
	mPattern = pcre_compile(pat,options,(const char **)&errptr,&erroffset,NULL);
	if(!mPattern) return false;
	//mPatternE = pcre_study(mPattern, 0, (const char **)&errptr);
	//if(!mPatternE) return false;
	return true;
}

void cPCRE::Clear()
{
	mPattern = NULL;
	mPatternE = NULL;
	mResult=0;
	if (!mCoords) mCoords = new int[3*mCoordsCount];
	//memset(mCoords,0, 3* mCoordsCount * sizeof(int) );
}

int nUtils::cPCRE::Exec(const string &text)
{
	mResult = pcre_exec(mPattern, mPatternE, text.c_str(), text.size(), 0, 0, mCoords, mCoordsCount);
	return mResult;
}

int cPCRE::Compare(int rank, const string &text, const char *text2)
{
	if(!this->PartFound(rank)) return -1;
	int start = mCoords[rank<<1];
	return StrCompare(text, start, mCoords[(rank<<1)+1]-start,text2);
}

int cPCRE::Compare(int rank, const string &text, const string &text2)
{
	return Compare(rank, text, text2.c_str());
}

int cPCRE::Compare(const string &name, const string &text, const string &text2)
{
	return Compare(this->GetStringRank(name), text, text2.c_str());
}

void cPCRE::Extract(int rank, const string &src, string &dst)
{
	if(!this->PartFound(rank)) return;
	int start = mCoords[rank<<1];
	dst.assign(src, start, mCoords[(rank<<1)+1]-start);
}

bool cPCRE::PartFound(int rank)
{
	if((rank < 0)|| (rank >= mResult)) return false;

	return mCoords[rank<<1] >= 0;
}

void cPCRE::Replace(int rank, string &InString, const string &ByThis)
{
	if(!this->PartFound(rank)) return;
	int start = mCoords[rank<<1];
	InString.replace(start, mCoords[(rank<<1)+1]-start, ByThis);
}

int cPCRE::GetStringRank(const string &name)
{
#if HAVE_PCRE_GET_STRINGNUMBER
	return pcre_get_stringnumber(this->mPattern, name.c_str());
#else
	return -1;
#endif
}
	}; // namespace nUtils
}; // namespace nVerliHub
