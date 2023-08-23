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

void IPlayer::Close()
{
    mux.lock();
    //1先关闭主题线程， 再清理观察者
    //同步线程；
    XThread::Stop();
    //解封装 清理
    if(demux)
        demux->Stop();
    //解码
    if(vdecode)
        vdecode->Stop();
    if(adecode)
        adecode->Stop();
    if(audioPlay)
        audioPlay->Stop();

    //2清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();
    //3清理资源
    if(audioPlay)
        audioPlay->Closer();
    if(videoView)
        videoView->Close();
    if(adecode)
        adecode->Close();
    if(vdecode)
        vdecode->Close();
    if(demux)
        demux->Close();
    mux.unlock();
}



double IPlayer::PlayPos()
{
    double pos = 0.0;
    mux.lock();
    int total = 0;
    if(demux)
        total = demux->m_totalMs;
    if(total > 0)
    {
        if(vdecode)
        {
            pos = (double)vdecode->m_curPts / (double)total;
        }
    }

    mux.unlock();
    return pos;
}

bool IPlayer::Seek(double pos)
{
    bool b = false;
    if(!demux)
        return false;
    //暂停所有线程
    SetPause(true); //里面有锁所以不要和当前锁一起用

    mux.lock();
    //清理缓冲
    //2清理缓冲队列
    if(vdecode)
        vdecode->Clear();
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    if(demux)
        b = demux->Seek(pos);//seek转跳到关键帧了
    if(!vdecode)
    {
        mux.unlock();
        SetPause(false);
        return b;
    }
    //解码到实际需要显示的帧
    int seekPts = pos* demux->m_totalMs;
    while(!isExit)
    {
        XData pkt = demux->read();
        if(pkt.size <=0 )break;
        if(pkt.isAudio)
        {
            if(pkt.pts < seekPts)
            {
                pkt.Drop();
                continue;
            }
            //写入缓冲队列
            demux->Notify(pkt);
            //pkt.Drop();
            continue;
        }
        //旧恶吗需要显示的帧之前的数据
        vdecode->SendPacket(pkt);
        pkt.Drop();
        XData data = vdecode->ReceiveFrame();
        if(data.size <= 0)
        {
            continue;
        }

        if(data.pts >= seekPts)
        {
            //vdecode->Notify(data);
            break;
        }
    }
    mux.unlock();
    SetPause(false);
    return b;
}

bool IPlayer::Open(const char *filePath) {
    Close();
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
    //if(outPara.sample_rate <= 0)
        outPara = demux->GetAPara();

    if(!resample || !resample->open(demux->GetAPara(), outPara))
    {
        XLOGE("resample is null %d resample->Open %s failed",resample == nullptr, filePath);
    }

    XLOGE("open success!!!!");
    mux.unlock();
    return true;
}
void IPlayer::SetPause(bool isP) {
    mux.lock();
    XThread::SetPause(isP);
    if(demux)
        demux->SetPause(isP);
    if(vdecode)
        vdecode->SetPause(isP);
    if(adecode)
        adecode->SetPause(isP);
    if(audioPlay)
        audioPlay->SetPause(isP);

    mux.unlock();
}


bool IPlayer::Start() {

    mux.lock();
    if(audioPlay)
        audioPlay->StartPlay(outPara);
    if(adecode)
        adecode->Start();
    if(vdecode)
        vdecode->Start();

    if(!demux || !demux->Start())
    {
        XLOGE("demux->Start failed ");
        mux.unlock();
        return false;
    }

    XThread::Start();

    mux.unlock();
    return true;
}

bool IPlayer::InitView(void *pWin) {

    if(videoView)
    {
        videoView->Close();
        videoView->SetRender(pWin);
    }

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





