//
// Created by John on 2023-08-06.
//

#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H


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
    virtual bool Start();
    virtual bool InitView(void* pWin);

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
};


#endif //XPLAY_IPLAYER_H
