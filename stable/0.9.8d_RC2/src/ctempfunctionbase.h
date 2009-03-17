/***************************************************************************
 *   Copyright (C) 2003 by Dan Muller                                      *
 *   dan at verliba dot cz                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NUTILSCTEMPFUNCTIONBASE_H
#define NUTILSCTEMPFUNCTIONBASE_H

namespace nUtils {

/**
the interface for temporary functions that are periodialy called to do some stuff, until they are done

@author Daniel Muller
*/
class cTempFunctionBase{
public:
    cTempFunctionBase();
    virtual ~cTempFunctionBase();
    virtual bool done() = 0;
    virtual void step() = 0;
};

};

#endif
