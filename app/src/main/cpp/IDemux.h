//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"

class IDemux {
public:
    //定义打开的接口
    virtual bool open(const char* rul) = 0;
    //定义关闭的接口
    virtual XData close() = 0;

protected:
    long m_totalMs = 0; // 总时长
};


#endif //XPLAY_IDEMUX_H
