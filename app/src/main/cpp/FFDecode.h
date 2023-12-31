//
// Created by John on 2023-07-12.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "IDecode.h"
struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode{
public:
    static void InitHard(void *vm);
    virtual bool Open(XParameter para, bool isHard = false) ;
    virtual void Close();
    virtual void Clear();
    virtual bool SendPacket(XData pkt);
    //再次调用 会复用上次 空间 线程不安全
    virtual XData ReceiveFrame();

protected:
    AVCodecContext * m_pCodecContext = 0;
    AVFrame*         m_pFrame = 0;     //这个在ffmpeg 中如果地址不变他会一直重复使用
    std::mutex       m_mux;
};


#endif //XPLAY_FFDECODE_H
