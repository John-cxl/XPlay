//
// Created by John on 2023-07-05.
//

#include "XData.h"

extern "C"{
#include <libavcodec/avcodec.h>
};

void XData::Drop()
{
    if(nullptr == pData){return;}
    av_packet_free((AVPacket **)&pData);
    pData = nullptr;
    size = 0;
}