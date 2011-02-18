#ifndef CPIISP_H
#define CPIISP_H

#include "src/tlistplugin.h"
#include "cisps.h"
#include "cconsole.h"

using namespace nDirectConnect;
using namespace nDirectConnect::nPlugin;

typedef tpiListPlugin<cISPs,cISPConsole> tpiISPBase;

class cpiISP : public tpiISPBase
{
public:
	cpiISP();
	virtual ~cpiISP();
	virtual void OnLoad(cServerDC *server);

	virtual bool RegisterAll();
	virtual bool OnParsedMsgMyINFO(cConnDC * conn, cMessageDC *mess);
	virtual bool OnParsedMsgValidateNick(cConnDC * conn, cMessageDC *mess);
	virtual bool OnOperatorCommand(cConnDC *, string *);
	cISPCfg *mCfg;
};

#endif
