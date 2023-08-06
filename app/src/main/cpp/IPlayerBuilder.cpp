//
// Created by John on 2023-08-06.
//

#include "IPlayerBuilder.h"

#include "IDecode.h"
#include "IResample.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
#include "IDemux.h"


IPlayer *IPlayerBuilder::BuilderPlayer(unsigned char index)
{
    IPlayer *play = CreatePlayer(index);

    //解封装
    IDemux* pDemux = CreateDemux();
    //视频解码
    IDecode* pVDecode = CreateDecode();
    //音频解码
    IDecode* pADecode = CreateDecode();
    //解码器观察解封装
    pDemux->AddObserver(pVDecode);  //添加观察者
    pDemux->AddObserver(pADecode);  //添加观察者
    //显示观察视频解码器
    IVideoView* view = CreateVideoView();
    pVDecode->AddObserver(view);
    //重采样观察音频解码器
    IResample * pResample = CreateResample();
    pADecode->AddObserver(pResample);   //音频解封装 添加观察者
    //音频观察 重采样
    IAudioPlay *pAudioPlay = CreateAudioPlay();
    pResample->AddObserver(pAudioPlay); //重采样    添加观察者AudioPlay
    IPlayer::Get()->demux = pDemux;
    IPlayer::Get()->vdecode = pVDecode;
    IPlayer::Get()->adecode = pADecode;
    IPlayer::Get()->videoView = view;
    IPlayer::Get()->resample = pResample;
    IPlayer::Get()->audioPlay = pAudioPlay;  //这个是注入  外部注入

    return play;
}
