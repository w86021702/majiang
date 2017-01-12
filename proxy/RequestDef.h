#ifndef __REQUEST_DEF_H__
#define __REQUEST_DEF_H__

#include <string.h>

enum EPacket_Type
{
    EPacket_Type_INPUT = 1,
    EPacket_Type_OUTPUT = 2,
};

struct SSPacket
{
    SSPacket()
    {
        memset(this, 0, sizeof(SSPacket));
    }
    unsigned int uid; //主键
    unsigned int clientId;
    unsigned int cmd;
    unsigned int type;
    unsigned int len;
    long long time;
};
const unsigned int SSPacket_Size = sizeof(SSPacket);


struct CSPacket
{
    CSPacket()
    {
        memset(this, 0, sizeof(CSPacket));
    }
    unsigned int clientType;
    unsigned int cmd;
    unsigned int checksum;
    unsigned int len;
    long long time;
};
const unsigned int CSPacket_Size = sizeof(CSPacket);


#endif
