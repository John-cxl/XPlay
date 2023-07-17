//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"
#include "XThread.h"
#include "IObserver.h"
#include "XParameter.h"

class IDemux :public IObserver {
public:
    //定义打开的接口
    virtual bool open(const char* rul) = 0;
    virtual XParameter GetVPara() = 0;
    virtual XParameter GetAPara() = 0;

    //定义关闭的接口
    virtual XData close() = 0;
    virtual XData read()  = 0;
protected:
    long m_totalMs = 0; // 总时长

    virtual void Main();
};


#endif //XPLAY_IDEMUX_H
