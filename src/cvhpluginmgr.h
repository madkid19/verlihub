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
#ifndef NDIRECTCONNECT_NPLUGINCVHPLUGINMGR_H
#define NDIRECTCONNECT_NPLUGINCVHPLUGINMGR_H
#include "cpluginmanager.h"
#include "ccallbacklist.h"
#include "cvhplugin.h"

namespace nVerliHub {

	namespace nSocket {
		class cServerDC;
	};
	class cUser;
	class cDCTag;
	class cUserCollection;

	namespace nProtocol {
		class cMessageDC;
	};
/**
  * Verlihub's plugin namespace
  * contains baase classes fo plugin related structures specialized for verlihub
  */
	namespace nPlugin {

/**
verlihub's plugin manager

@author Daniel Muller
*/

class cVHPluginMgr : public cPluginManager
{
public:
	cVHPluginMgr(nSocket::cServerDC *,const string pluginDir);
	virtual ~cVHPluginMgr();
	virtual void OnPluginLoad(cPluginBase *pi);
private:
	nSocket::cServerDC *mServer;
};


/**
  * \brief Verlihub CallBackList Base class
  */
class cVHCBL_Base : public cCallBackList
{
public:
	cVHCBL_Base(cVHPluginMgr *mgr, const char * id ) : cCallBackList (mgr, string(id)) {}
	/** Call one verlihub plugin's callback */
	virtual bool CallOne(cPluginBase *pi) { return CallOne((cVHPlugin*)pi); }
	/** Call a specific callback for verlihub plugin */
	virtual bool CallOne(cVHPlugin *pi) = 0;
};

class cVHCBL_Simple : public cVHCBL_Base
{
public:
	typedef bool (cVHPlugin::*tpf0TypeFunc)();
protected:
	tpf0TypeFunc m0TFunc;
public:
	/** Constructor */
	cVHCBL_Simple( cVHPluginMgr *mgr, const char *id, tpf0TypeFunc pFunc ) :
		cVHCBL_Base (mgr, id), m0TFunc(pFunc) {}

	virtual bool CallOne(cVHPlugin *pi) { return (pi->*m0TFunc)(); }
};

template <class Type1> class tVHCBL_1Type : public cVHCBL_Base
{
public:
	typedef bool (cVHPlugin::*tpf1TypeFunc)(Type1 *);
protected:
	tpf1TypeFunc m1TFunc;
	Type1 *mData1;
public:
	/** Constructor */
	tVHCBL_1Type( cVHPluginMgr *mgr, const char *id, tpf1TypeFunc pFunc ) :
	cVHCBL_Base (mgr, id), m1TFunc(pFunc) { mData1 = NULL;}

	virtual bool CallOne(cVHPlugin *pi) { return (pi->*m1TFunc)(mData1); }
	virtual bool CallAll(Type1 *par1)
	{
		mData1 = par1;
		if ((mData1 != NULL) )
			return this->cCallBackList::CallAll();
		else  return false;
	}

};


template <class Type1, class Type2> class tVHCBL_2Types : public cVHCBL_Base
{
public:
	typedef bool (cVHPlugin::*tpf2TypesFunc)(Type1 *, Type2 *);
protected:
	tpf2TypesFunc m2TFunc;
	Type1 *mData1;
	Type2 *mData2;
public:
	/** Constructor */
	tVHCBL_2Types( cVHPluginMgr *mgr, const char *id, tpf2TypesFunc pFunc ) :
	cVHCBL_Base (mgr, id), m2TFunc(pFunc) { mData1 = NULL; mData2 = NULL;}
	virtual ~tVHCBL_2Types() {}

	virtual bool CallOne(cVHPlugin *pi) { return (pi->*m2TFunc)(mData1, mData2); }
	virtual bool CallAll(Type1 *par1, Type2 *par2)
	{
		mData1 = par1;
		mData2 = par2;
		if ((mData1 != NULL) && (mData2 !=NULL))
			return this->cCallBackList::CallAll();
		else  return false;
	}

};

template <class Type1, class Type2> class tVHCBL_R2Types : public cVHCBL_Base
{
	public:
		typedef bool (cVHPlugin::*tpfR2TypesFunc)(Type1 , Type2 );
	protected:
		tpfR2TypesFunc m2TFunc;
		Type1 mData1;
		Type2 mData2;
	public:
		/** Constructor */
	tVHCBL_R2Types( cVHPluginMgr *mgr, const char *id, tpfR2TypesFunc pFunc ) :
		cVHCBL_Base (mgr, id), m2TFunc(pFunc) { }
	virtual ~tVHCBL_R2Types() {}

	virtual bool CallOne(cVHPlugin *pi) { return (pi->*m2TFunc)(mData1, mData2); }
	virtual bool CallAll(Type1 par1, Type2 par2)
	{
		mData1 = par1;
		mData2 = par2;
		return this->cCallBackList::CallAll();
	}

};

template <class Type1, class Type2, class Type3> class tVHCBL_3Types : public cVHCBL_Base
{
public:
	typedef bool (cVHPlugin::*tpf3TypesFunc)(Type1 , Type2 , Type3);
protected:
	tpf3TypesFunc m3TFunc;
	Type1 mData1;
	Type2 mData2;
	Type3 mData3;
public:
	/** Constructor */
	tVHCBL_3Types( cVHPluginMgr *mgr, const char *id, tpf3TypesFunc pFunc ) :
	cVHCBL_Base (mgr, id), m3TFunc(pFunc) { }
	virtual ~tVHCBL_3Types() {}

	virtual bool CallOne(cVHPlugin *pi) { return (pi->*m3TFunc)(mData1, mData2, mData3); }
	virtual bool CallAll(Type1 par1, Type2 par2, Type3 par3)
	{
		mData1 = par1;
		mData2 = par2;
		mData3 = par3;
		return this->cCallBackList::CallAll();
	}

};

template <class Type1, class Type2, class Type3, class Type4> class tVHCBL_4Types: public cVHCBL_Base
{
public:
	typedef bool (cVHPlugin::*tpf4TypesFunc)(Type1 *, Type2 *, Type3, Type4);
protected:
	tpf4TypesFunc m4TFunc;
	Type1 *mData1;
	Type2 *mData2;
	Type3 mData3;
	Type4 mData4;
public:
	tVHCBL_4Types(cVHPluginMgr *mgr, const char *id, tpf4TypesFunc pFunc):
	cVHCBL_Base(mgr, id), m4TFunc(pFunc) {mData1 = NULL; mData2 = NULL;}
	virtual ~tVHCBL_4Types() {}
	virtual bool CallOne(cVHPlugin *pi) {return (pi->*m4TFunc)(mData1, mData2, mData3, mData4);}

	virtual bool CallAll(Type1 *par1, Type2 *par2, Type3 par3, Type4 par4)
	{
		mData1 = par1;
		mData2 = par2;
		mData3 = par3;
		mData4 = par4;

		if ((mData1 != NULL) && (mData2 != NULL))
			return this->cCallBackList::CallAll();
		else
			return false;
	}
};

/**
  * \brief Verlihub CallBackList with a single connection parameter
  */
class cVHCBL_Connection : public cVHCBL_Base
{
public:
	typedef bool (cVHPlugin::*tpfConnFunc)(nSocket::cConnDC *);
protected:
	tpfConnFunc mFunc;
	nSocket::cConnDC *mConn;
public:
	/** Constructor */
	cVHCBL_Connection( cVHPluginMgr *mgr, const char *id, tpfConnFunc pFunc ) :
		cVHCBL_Base (mgr, id), mFunc(pFunc) { mConn = NULL;}

	virtual bool CallOne(cVHPlugin *pi) { return (pi->*mFunc)(mConn); }
	virtual bool CallAll(nSocket::cConnDC *conn)
	{
		mConn = conn;
		if(mConn != NULL) return this->cCallBackList::CallAll();
		else  return false;
	}

};

typedef tVHCBL_4Types<nSocket::cConnDC, std::string, int, int> cVHCBL_ConnTextIntInt;
typedef tVHCBL_3Types<cUser *, cUser *,  std::string *> cVHCBL_UsrUsrStr;
typedef tVHCBL_3Types<std::string, int ,  int> cVHCBL_StrIntInt;
typedef tVHCBL_3Types<std::string, std::string ,  std::string> cVHCBL_StrStrStr;
typedef tVHCBL_2Types<nSocket::cConnDC, nProtocol::cMessageDC> cVHCBL_Message;
typedef tVHCBL_2Types<cUser, cUser> cVHCBL_UsrUsr;
typedef tVHCBL_2Types<nSocket::cConnDC, cDCTag> cVHCBL_ConnTag;
typedef tVHCBL_2Types<nSocket::cConnDC, std::string> cVHCBL_ConnText;
typedef tVHCBL_R2Types<std::string, int> cVHCBL_StringInt;
typedef tVHCBL_R2Types<std::string, std::string> cVHCBL_Strings;
typedef tVHCBL_1Type<string> cVHCBL_String;
typedef tVHCBL_1Type<cUser> cVHCBL_User;
typedef tVHCBL_1Type<nTables::cBan> cVHCBL_Ban;

	}; // namespace nPlugin
}; // namespace nVerliHub

#endif
