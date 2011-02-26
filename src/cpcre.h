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
#ifndef NUTILSCPCRE_H
#define NUTILSCPCRE_H
#include <pcre.h>
#include <string>
using std::string;

namespace nUtils {

/**
class wrapper for pcre API
with usage optimized for some special cases, to be specified :o)
@author Daniel Muller
*/

class cPCRE
{
public:
	cPCRE(int coord=30);
	~cPCRE();
	cPCRE(const char *, unsigned int options, int coord=30);
	cPCRE(const std::string&, unsigned int options, int coord=30);
	bool Compile(const char *, unsigned int options = 0);
	int Exec(const std::string &text);
	int Compare(const std::string &name, const std::string &text, const string &text2);
	int Compare(int rank, const std::string &text, const string &text2);
	int Compare(int rank, const std::string &text, const char   *text2);
	int StartOf(int rank){ if(rank < 0 || rank >= mResult) return -1; return mCoords[rank << 1];}
	void Extract( int rank, const std::string &src, std::string &dst);
	void Replace(int rank, std::string &InString, const std::string &ByThis);
	int GetStringRank(const std::string &ame);
	bool PartFound(int index);
private:
	pcre * mPattern;
	pcre_extra * mPatternE;
	int mResult;
	int *mCoords;
	int mCoordsCount;
private:
	void Clear();
};

};

#endif
