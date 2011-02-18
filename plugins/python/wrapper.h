
//
// Python Wrapper for Verlihub Python Plugin v.1.1
//
// Created in 2007 by Frog, frg@otaku-anime.net
// Code licensed under the GNU Library General Public License
//

#ifndef WRAPPER_H
#define WRAPPER_H

extern "C"
{
	#include <Python.h>
	#include <stdio.h>
	#include <stdlib.h>
}

#include <string>
#include <sstream>
#include <vector>

// user rights
#define w_UR_CHAT     0x0001
#define w_UR_PM       0x0002
#define w_UR_SEARCH   0x0004
#define w_UR_CTM      0x0008

#define freee(x) if (x) free((void*)x)

// a string the user would not write
#define W_NOSTRING = "SG083tcs0ODEgbns623OFfew"

// function positions in the hook table:
enum { W_OnNewConn, W_OnCloseConn, W_OnParsedMsgChat, W_OnParsedMsgPM, W_OnParsedMsgSearch, W_OnParsedMsgSR, W_OnParsedMsgMyINFO, W_OnParsedMsgValidateNick, W_OnParsedMsgAny, W_OnParsedMsgSupport, W_OnParsedMsgMyPass, W_OnParsedMsgConnectToMe, W_OnParsedMsgRevConnectToMe, W_OnUnknownMsg, W_OnOperatorCommand, W_OnOperatorKicks, W_OnOperatorDrops, W_OnValidateTag, W_OnUserCommand, W_OnUserLogin, W_OnUserLogout, W_OnTimer, W_OnNewReg, W_OnNewBan };

// MAX_HOOKS must be more than the number of elements in above enum
const int W_MAX_HOOKS = 50;

// function positions in the callback table
enum { W_SendDataToUser, W_SendDataToAll, W_SendPMToAll, W_CloseConnection, W_GetMyINFO, W_SetMyINFO, W_GetUserClass, W_GetUserHost, W_GetUserIP, W_GetUserCC, W_GetNickList, W_GetOpList, W_Ban, W_KickUser, W_ParseCommand, W_SetConfig, W_GetConfig, W_AddRobot, W_DelRobot, W_SQL, W_SQLQuery, W_SQLFetch, W_SQLFree, W_GetUsersCount, W_GetTotalShareSize, W_UserRestrictions, W_Topic, W_mc, W_usermc, W_classmc, W_pm };

// the maximum number of values that can be returned inside w_Treturn
#define W_MAX_RETVALS 10

// possible return values, as follows: nothing, long, double, char*, void*, char**
enum { w_ret_none, w_ret_int, w_ret_float, w_ret_char, w_ret_void, w_ret_tab };

// MAX_CALLBACKS must be more than the number of elements in above enum
const int W_MAX_CALLBACKS = 50;

typedef struct
{
	char type;
	union {
		long    l;
		char   *s;
		double  d;
		void   *p;
	};
} w_Telement;

typedef struct
{
	const char *format;
	w_Telement args[];
} w_Targs;

typedef w_Targs*   (*w_Tcallback) (int, w_Targs*);

typedef struct {
	int             id;
	PyThreadState * state;
	const char *    path;
	const char *    name;
	w_Tcallback *   callbacks;
	char *          hooks;
	const char *    botname;
	const char *    opchatname;
} w_TScript;

// the following functions are all you need to use in the plugin that loads this wrapper

extern "C"
{
int         w_Begin    (w_Tcallback* callbacks);
int         w_End      ();
int         w_ReserveID();
int         w_Load     (w_Targs* args);
int         w_Unload   (int id);
int         w_HasHook  (int id, int hook);
w_Targs    *w_CallHook (int id, int num, w_Targs *params);   // non-empty / non-zero return means further processing by other plugins or the hub
PyObject   *w_GetHook  (int hook);
const char *w_HookName (int hook);
const char *w_CallName (int callback);
w_Targs*    w_pack     (const char* format, ... );
int         w_unpack   (w_Targs* a, const char* format, ... );
void        w_LogLevel (int level);
const char *w_packprint (w_Targs* a);
}

// typedefs for pointers to functions to be used in plugin via dlsym()
typedef int         (*w_TBegin)     (w_Tcallback*);
typedef int         (*w_TEnd)       (void);
typedef int         (*w_TReserveID) (void);
typedef int         (*w_TLoad)      (w_Targs*);
typedef int         (*w_TUnload)    (int);
typedef int         (*w_THasHook)   (int, int);
typedef w_Targs*    (*w_TCallHook)  (int, int, w_Targs *);
typedef char *      (*w_THookName)  (int);
typedef const char* (*w_TCallName)  (int callback);
typedef w_Targs*    (*w_Tpack)      (const char*, ... );
typedef int         (*w_Tunpack)    (w_Targs*, const char*, ... );
typedef void        (*w_TLogLevel)  (int);
typedef const char* (*w_Tpackprint) (w_Targs*);

#endif
