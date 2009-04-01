#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>

#if (!defined _WIN32) && (!defined __int64)
#define __int64  long long
#endif

using namespace std;
namespace nStringUtils
{

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
string Simplify(unsigned long val);
string StringFrom(__int64 const &ll);
__int64 StringAsLL(const string &);
int CountLines(const string &);
bool LimitLines(const string &str, int max);
};

#endif
