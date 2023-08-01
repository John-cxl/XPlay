//
// Created by John on 2023-07-05.
//

#include "XData.h"

extern "C"{
#include <libavcodec/avcodec.h>
};

void XData::Drop()
{
    if(!pData){return;}
    if(AVPACKET_TYPE == type)
    {
        av_packet_free((AVPacket **)&pData);
    }
    else
    {
        delete pData;
    }

    pData = nullptr;
    size = 0;
}

bool XData::Alloc(int size, const char *data) {
    Drop();
    type = UCHAR_TYPE;
    if(size <= 0)return false;
    this->pData = new unsigned char[size];
    if(!this->pData)return false;
    if(data)
    {
        memcpy(this->pData, data, size);
    }

    return true;
}
