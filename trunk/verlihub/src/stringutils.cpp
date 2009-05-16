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

#include "stringutils.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <sys/param.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctype.h>

namespace nStringUtils
{

int StrCompare(const string &str1, int start, int count, const string &str2)
{
	#if !defined OLD_STD_STRING
	return str1.compare(start,count,str2);
	#else
	return str2.compare(str1, start, count);
	#endif
}

string toLower(const string str)
{
	string dest = str;
	transform(str.begin(), str.end(), dest.begin(), ::tolower);
	return dest;
}

string toUpper(const string str)
{
	string dest = str;
	transform(str.begin(), str.end(), dest.begin(), ::toupper);
	return dest;
}

void ShrinkStringToFit(string &str)
{
	std::string(str.data(), str.size()).swap(str);
}

void StrCutLeft(string &str, size_t cut)
{
	string tmp;
	if(cut > str.length()) cut = str.length();
	//cout << str.size() << " " << str.capacity() << " => ";
	std::string(str, cut, str.size() - cut).swap(str);
	//cout << str.size() << " " << str.capacity() << endl;
}

void StrCutLeft(const string &str1, string &str2, size_t cut)
{
	string tmp;
	//cout << str2.size() << " " << str2.capacity() << " => ";
	if(cut > str1.size()) cut = str1.size();
	std::string(str1, cut, str1.size() - cut).swap(str2);	
	//cout << str2.size() << " " << str2.capacity() << endl;
}

bool LoadFileInString(const string &FileName, string &dest)
{
	string buf;
	bool AddLine = false;
	ifstream is(FileName.c_str());

	if(!is.is_open()) return false;
	while(!is.eof())
	{
		getline(is, buf);
		if (AddLine) dest += "\r\n";
		else AddLine = true;
		dest+= buf;
	}
	is.close();
	return true;
}

void ExpandPath(string &Path)
{
	if(Path.substr(0,2) == "./") {
		string tmp = Path;
#if defined HAVE_LINUX
		Path = get_current_dir_name();
#elif defined HAVE_FREEBSD
		Path = getcwd(NULL, MAXPATHLEN);
#else
		Path = GetCurrentDirectory(); //FIXME
#endif
		Path += "/" + tmp.substr(2,tmp.length());
	}
	size_t pos;
#if ! defined _WIN32
	pos = Path.find("~");
	if(pos != Path.npos) {
		Path.replace(pos, 2, getenv("HOME"));
	}
#endif
// FIXME: Windows uses \
	pos = Path.find("../");
	while (pos != Path.npos) {
		Path.replace(pos, 3, "");
		pos = Path.find("../", pos);
	}
	int len = Path.length();
	if(Path.substr(len-1,len) != "/")
		Path.append("/");
}
void GetPath(const string FileName, string &Path, string &File)
{
	Path = FileName;
	size_t i = FileName.rfind("/");
	if(i != string::npos)
		Path = FileName.substr(0, i+1);
	File = FileName.substr(i+1);
}

void FilterPath(string &Path)
{
	size_t pos = Path.find("../");
	while (pos != Path.npos) {
		Path.replace(pos, 3, "");
		pos = Path.find("../", pos);
	}
}

/*!
    \fn ReplaceVarInString(const string&,const string &varname,string &dest, const string& by)
 */
void ReplaceVarInString(const string&src ,const string &varname, string &dest, const string& by)
{
	string searchvar("%[");
	searchvar+=varname;
	searchvar+="]";
	dest = src;
	size_t pos = dest.find(searchvar);
	while (pos != dest.npos)
	{
		dest.replace(pos, searchvar.size(), by);
		pos = dest.find(searchvar, pos);
	}
}


/*!
    \fn ReplaceVarInString(const string&,const string &varname,string &dest, int by)
 */
void ReplaceVarInString(const string &src,const string &varname,string &dest, int by)
{
	ostringstream os;
	os << by;
	ReplaceVarInString(src, varname, dest, os.str());
}


/*!
    \fn ReplaceVarInString(const string&,const string &varname,string &dest, double by)
 */
void ReplaceVarInString(const string&src,const string &varname,string &dest, double by)
{
	ostringstream os;
	os << by;
	ReplaceVarInString(src, varname, dest, os.str());
}


/*!
    \fn ReplaceVarInString(const string&,const string &varname,string &dest, long by)
 */
void ReplaceVarInString(const string &src,const string &varname,string &dest, long by)
{
	ostringstream os;
	os << by;
	ReplaceVarInString(src, varname, dest, os.str());
}

/*!
    \fn ReplaceVarInString(const string&,const string &varname,string &dest, __int64 by)
 */
void ReplaceVarInString(const string &src,const string &varname,string &dest, __int64 by)
{
	ReplaceVarInString(src, varname, dest, StringFrom(by));
}

string Simplify(unsigned long val)
{
	ostringstream os;
	float mb = static_cast<float>(val);
	if(mb >= 1024)
	{
		if(mb >= 1024 * 1024)
		{
			os << mb / 1024 * 1024 << "TB";
		}
		else
		{
			os << mb / 1024 << "GB";
		}
	}
	else
	{
		os << val << "MB";
	}
	return os.str();
}

string StringFrom(__int64 const &ll)
{
	char buf[32];
#ifdef _WIN32
	sprintf(buf,"%I64d",ll);
#else
	sprintf(buf,"%lld",ll);
#endif
	return buf;
}
__int64 StringAsLL(const string &str)
{
#ifdef _WIN32
	__int64 result;
	sscanf(str.c_str(),"%I64d",&result);
	return result;
#else
	return strtoll(str.c_str(),NULL,10);
#endif
}

int CountLines(const string &str)
{
	int lines=1;
	size_t pos=0;
	// the ?:  ... because in the beginning start search from the first char, but then from next after last found
	while(str.npos != (pos=str.find_first_of("\n",pos?pos+1:0))) 
		lines++;
	return lines;
}

bool LimitLines(const string &str, int max)
{
	int lines=1;
	size_t pos=0;
	// the ?:  ... because in the beginning start search from the first char, but then from next after last found
	while(str.npos != (pos=str.find_first_of("\n",pos?pos+1:0))) 
		if(++lines > max) 
			return false;
	return true;
}

};
