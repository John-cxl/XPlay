//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_FFDEMUX_H
#define XPLAY_FFDEMUX_H
#include "IDemux.h"

struct AVFormatContext; //这个AVFormat上下文需要 后面打开等操作使用

class FFDemux :public IDemux{
public:
    //定义打开的接口
    virtual bool open(const char* rul);
    //定义关闭的接口
    virtual XData close();
    //定义读取接口
    virtual XData read();


    //构造函数
    FFDemux();

private:
    AVFormatContext *ic = 0;
};


#endif //XPLAY_FFDEMUX_H
