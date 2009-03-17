#include "stringutils.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

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
