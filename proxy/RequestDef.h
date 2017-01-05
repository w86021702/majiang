#ifndef __REQUEST_DEF_H__
#define __REQUEST_DEF_H__

#include <string.h>

struct SSPacket
{
    SSPacket()
    {
        memset(this, 0, sizeof(SSPacket));
    }
    unsigned int uid;
    unsigned int clientId;
    unsigned int cmd;
    unsigned int checksum;
    unsigned int len;
    long long time;
};

const unsigned int SSPacket_Size = sizeof(SSPacket);



#endif
