//
// Created by John on 2023-07-12.
//

#ifndef XPLAY_IDECODE_H
#define XPLAY_IDECODE_H

#include "XParameter.h"
#include "IObserver.h"

//解码接口， 支持硬解码
class IDecode :public IObserver{
public:
    //打开解码器
    virtual bool Open(XParameter para) = 0;
};


#endif //XPLAY_IDECODE_H
