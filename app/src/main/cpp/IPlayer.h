//
// Created by John on 2023-08-06.
//

#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H

#include <mutex>
#include "XThread.h"
#include "XParameter.h"

class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;


class IPlayer : public XThread{
public:
    static IPlayer* Get(unsigned char index = 0);
    virtual bool Open(const char* filePath);
    virtual void Close();

    virtual bool Start();
    virtual bool InitView(void* pWin);
    //获取当前的播放进度  0.0 --- 1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);

    //是否硬解码
    bool isHardDecode = true;
    XParameter outPara;
    IDemux *demux = 0;
    IDecode * vdecode = 0;
    IDecode * adecode = 0;
    IResample * resample = 0;
    IVideoView * videoView = 0;
    IAudioPlay * audioPlay = 0;
protected:
    IPlayer() {};
    //用于音视频 同步
    void Main();
    std::mutex mux;
};


#endif //XPLAY_IPLAYER_H
