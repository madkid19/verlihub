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
#ifndef NUTILS_CTIME_H
#define NUTILS_CTIME_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <ostream>
#include <time.h>
#if defined _WIN32
#include <Winsock2.h>
#endif
#include <string>
#include <sys/time.h>

namespace nVerliHub {
	namespace nUtils {

class cTime : public timeval
{
	public:
	~cTime();
	cTime():mPrintType(0){Get();mPrintType = 0;}
	cTime(double sec){tv_sec=(long)sec; tv_usec=long((sec-tv_sec)*1000000); mPrintType = 0;};
	cTime(long sec, long usec=0){tv_sec=sec; tv_usec=usec; mPrintType = 0;};
	cTime(const cTime &t){tv_sec=t.tv_sec; tv_usec=t.tv_usec; mPrintType = 0;};
	int operator> (const cTime &t) const { if(tv_sec > t.tv_sec) return 1; if(tv_sec < t.tv_sec) return 0; return (tv_usec > t.tv_usec);};
	int operator>= (const cTime &t) const { if(tv_sec > t.tv_sec) return 1; if(tv_sec < t.tv_sec) return 0; return (tv_usec >= t.tv_usec);};
	int operator< (const cTime &t) const { if(tv_sec < t.tv_sec) return 1; if(tv_sec > t.tv_sec) return 0; return (tv_usec < t.tv_usec);};
	int operator<= (const cTime &t) const { if(tv_sec < t.tv_sec) return 1; if(tv_sec > t.tv_sec) return 0; return (tv_usec <= t.tv_usec);};
	int operator== (const cTime &t) const { return ((tv_usec == t.tv_usec) && (tv_sec == t.tv_sec));};
	cTime & operator= (const cTime &t){ tv_usec = t.tv_usec; tv_sec = t.tv_sec; return *this;}
	cTime & Get(){gettimeofday(this,NULL);return *this;}
	cTime   operator- (const cTime &t) const {long sec = tv_sec-t.tv_sec; long usec=tv_usec-t.tv_usec; return cTime(sec,usec).Normalize();}
	cTime   operator+ (const cTime &t) const {long sec = tv_sec+t.tv_sec; long usec=tv_usec+t.tv_usec; return cTime(sec,usec).Normalize();}
	cTime   operator+ (int _sec) const {long sec = tv_sec+_sec; return cTime(sec,tv_usec).Normalize();}
	cTime   operator- (int _sec) const {long sec = tv_sec-_sec; return cTime(sec,tv_usec).Normalize();}
	cTime & operator-= (const cTime &t){tv_sec-=t.tv_sec;tv_usec-=t.tv_usec; Normalize(); return *this;}

	cTime & operator+= (const cTime &t){tv_sec+=t.tv_sec;tv_usec+=t.tv_usec; Normalize(); return *this;}
	cTime & operator+= (int msec){tv_usec+=1000*msec; Normalize(); return *this;}
	cTime & operator+= (long usec){tv_usec+=usec; Normalize(); return *this;}

	cTime & operator/= (int i){long sec=tv_sec/i; tv_usec+=1000000*(tv_sec % i); tv_usec/=i; tv_sec=sec; Normalize(); return *this;}
	cTime & operator*= (int i){tv_sec*=i;tv_usec*=i;Normalize(); return *this;}
	cTime   operator/ (int i) const {long sec=tv_sec/i; long usec=tv_usec+1000000*(tv_sec % i); usec/=i; return cTime(sec,usec).Normalize();};
	cTime   operator* (int i) const {long sec=tv_sec*i; long usec=tv_usec*i; return cTime(sec,usec).Normalize();};
	operator double(){ return double(tv_sec)+double(tv_usec)/1000000.;};
	operator long() const { return long(tv_sec)*1000000+long(tv_usec);};
	operator bool() const { return !(!tv_sec && !tv_usec);}
	int operator! () { return !tv_sec && !tv_usec;}
	long Sec() const{return tv_sec;}
	unsigned long MiliSec(){return (unsigned long)(tv_sec)*1000+(unsigned long)(tv_usec)/1000;}

	cTime & Normalize()
	{
		if(tv_usec >= 1000000 || tv_usec <= -1000000)
		{
			tv_sec += tv_usec/1000000;
			tv_usec %= 1000000;
		}
		if( tv_sec < 0 && tv_usec > 0)
		{
			tv_sec ++;
			tv_usec-= 1000000;
		}
		if( tv_sec > 0 && tv_usec < 0)
		{
			tv_sec --;
			tv_usec+= 1000000;
		}
		return *this;
	};

	std::string AsString() const;
	friend std::ostream & operator<< (std::ostream &os, const cTime &t);
private:
	mutable int mPrintType;
public:
	const cTime & AsDate() const { mPrintType=1; return *this;}
	const cTime & AsPeriod() const { mPrintType=2; return *this;}

};

	}; // namespace nUtils
}; // namespace nVerliHub
#endif
