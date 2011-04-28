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

		cPCRE::cPCRE(int offsetResultSize) :
	mOffsetResults(NULL),
	mOffsetResultSize(offsetResultSize)
{
	Clear();
}

cPCRE::~cPCRE(){
	if(mOffsetResults) delete [] mOffsetResults;
	mOffsetResults = NULL;
}

cPCRE::cPCRE(const char *pattern, unsigned int options, int offsetResultSize) :
	mOffsetResults(NULL),
	mOffsetResultSize(offsetResultSize)
{
	Clear();
	Compile(pattern, options);
}

cPCRE::cPCRE(const string &pattern, unsigned int options, int coord):
	mOffsetResults(NULL),
	mOffsetResultSize(coord)
{
	Clear();
	Compile(pattern.c_str(), options);
}

bool cPCRE::Compile(const char *pattern, unsigned int options)
{
	char *errptr;
	int erroffset;
	mPattern = pcre_compile(pattern, options, (const char **)&errptr, &erroffset, NULL);
	return mPattern != NULL;
}

void cPCRE::Clear()
{
	mPattern = NULL;
	mResult = 0;
	if(!mOffsetResults)
		mOffsetResults = new int[3*mOffsetResultSize];
}

int cPCRE::Exec(const string &subject)
{
	mResult = pcre_exec(mPattern, NULL, subject.c_str(), subject.size(), 0, 0, mOffsetResults, mOffsetResultSize);
	return mResult;
}

int cPCRE::Compare(int index, const string &text, const char *text2)
{
	if(!this->PartFound(index))
		return -1;
	int start = mOffsetResults[index<<1];
	return StrCompare(text, start, mOffsetResults[(index<<1)+1]-start,text2);
}

int cPCRE::Compare(int index, const string &text, const string &text2)
{
	return Compare(index, text, text2.c_str());
}

int cPCRE::Compare(const string &name, const string &text, const string &text2)
{
	return Compare(this->GeStringNumber(name), text, text2.c_str());
}

void cPCRE::Extract(int index, const string &src, string &dst)
{
	if(!this->PartFound(index))
		return;
	int start = mOffsetResults[index<<1];
	dst.assign(src, start, mOffsetResults[(index<<1)+1]-start);
}

bool cPCRE::PartFound(int index)
{
	if((index < 0)|| (index >= mResult)) return false;

	return mOffsetResults[index<<1] >= 0;
}

void cPCRE::Replace(int index, string &subject, const string &replace)
{
	if(!this->PartFound(index)) return;
	int start = mOffsetResults[index<<1];
	subject.replace(start, mOffsetResults[(index<<1)+1]-start, replace);
}

int cPCRE::GeStringNumber(const string &substring)
{
	return pcre_get_stringnumber(this->mPattern, substring.c_str());
}
	}; // namespace nUtils
}; // namespace nVerliHub
