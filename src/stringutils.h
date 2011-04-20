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
#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string>
#if (!defined _WIN32) && (!defined __int64)
#define __int64  long long
#endif

using namespace std;
namespace nVerliHub {
	namespace nUtils {

int StrCompare(const string &str1, int Start, int Count, const string &str2);
string toLower(const std::string str);
string toUpper(const std::string str);
void ShrinkStringToFit(string &str);
void StrCutLeft(string &, size_t);
void StrCutLeft(const string &str1, string &str2, size_t cut);
bool LoadFileInString(const string &FileName, string &dest);
void GetPath(const string FileName, string &Path, string &File);
void FilterPath(string &Path);
void ExpandPath(string &Path);
void ReplaceVarInString(const string &src,const string &var,string &dest, const string& by);
void ReplaceVarInString(const string &src,const string &var,string &dest, double by);
void ReplaceVarInString(const string &src,const string &var,string &dest, int by);
void ReplaceVarInString(const string &src,const string &var,string &dest, long by);
void ReplaceVarInString(const string &src,const string &var,string &dest, __int64 by);
string convertByte(__int64 byte, bool UnitType);
string StringFrom(__int64 const &ll);
__int64 StringAsLL(const string &);
int CountLines(const string &);
bool LimitLines(const string &str, int max);

	}; // namespace nUtils
}; // namespace nVerliHub

#endif
