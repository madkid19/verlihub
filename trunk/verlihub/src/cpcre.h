/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
