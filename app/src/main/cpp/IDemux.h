//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"
#include "XThread.h"
#include "IObserver.h"
#include "XParameter.h"
#include <mutex>
class IDemux :public IObserver {
public:
    //定义打开的接口
    virtual bool open(const char* rul) = 0;
    //seek 位置 pos 0.0 - 1.0
    virtual bool Seek(double pos) = 0;
    virtual void Close() = 0;
    virtual XParameter GetVPara() = 0;
    virtual XParameter GetAPara() = 0;
    //定义关闭的接口
    virtual XData read()  = 0;
    long m_totalMs = 0; // 总时长
protected:

    virtual void Main();
};


#endif //XPLAY_IDEMUX_H
