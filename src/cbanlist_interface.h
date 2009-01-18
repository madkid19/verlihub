#ifndef __CBANLIST_INTERFACE_H
#define __CBANLIST_INTERFACE_H

class cBanListInterface
{
public:
    cBanListInterface() {}
    virtual ~cBanListInterface() {}


private:
    cBanListInterface( const cBanListInterface& source );
    void operator = ( const cBanListInterface& source );
};


#endif // __CBANLIST_INTERFACE_H
