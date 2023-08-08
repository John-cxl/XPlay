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
    virtual void Close();
    virtual XParameter GetVPara();
    virtual XParameter GetAPara();
    //定义读取接口
    virtual XData read();

    //构造函数
    FFDemux();

private:
    AVFormatContext *m_pIc = 0;
    int              m_videoStreamIndex = 0; //一般 视频默认是0
    int              m_audioStreamIndex = 1; //一般 音频默认是1
};


#endif //XPLAY_FFDEMUX_H
