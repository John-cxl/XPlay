//
// Created by John on 2023-08-06.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
#include "XLog.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *filePath) {

    mux.lock();
    if(!demux || !demux->open(filePath))
    {
        XLOGE("demux is null %d demux->open %s file failed",demux == nullptr, filePath);
        mux.unlock();
        return false;
    }
    //解码 可能不需要， 解封装之后就是 原始数据
    if(!vdecode || !vdecode->Open(demux->GetVPara(),isHardDecode)){
        XLOGE("vdecode is null %d vdecode->Open %s failed",vdecode == nullptr, filePath);
        //return false;  //有时候不需要压缩 不用解码 不用返回
    }

    if(!adecode || !adecode->Open(demux->GetAPara())){
        XLOGE("adecode is null %d adecode->Open %s failed",adecode == nullptr, filePath);
        //return false;  //有时候不需要压缩 不用解码 不用返回
    }

    //重采样 有可能不需要，解码后或者解封装后可能是直接能播放的数据
    if(outPara.sample_rate <= 0)
        outPara = demux->GetAPara();

    if(!resample || !resample->open(demux->GetAPara(), outPara))
    {
        XLOGE("resample is null %d resample->Open %s failed",resample == nullptr, filePath);
    }

    XLOGE("open success!!!!");
    mux.unlock();
    return true;
}

bool IPlayer::Start() {

    mux.lock();
    if(!demux || !demux->Start())
    {
        XLOGE("demux->Start failed ");
        mux.unlock();
        return false;
    }

    if(adecode)
        adecode->Start();
    if(audioPlay)
        audioPlay->StartPlay(outPara);
    if(vdecode)
        vdecode->Start();
    XThread::Start();

    mux.unlock();
    return true;
}

bool IPlayer::InitView(void *pWin) {
    if(videoView)
        videoView->SetRender(pWin);
    return false;
}
//在 XThread  Start 中才会启动
void IPlayer::Main() {
    while(!isExit)
    {
        mux.lock();
        if(!audioPlay || !vdecode)
        {
            mux.unlock();
            XSleep(2);
            continue;
        }
        //同步
        vdecode->synPts = audioPlay->m_pts;
        mux.unlock();
        XSleep(2);
    }
}

