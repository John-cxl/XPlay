//
// Created by John on 2023-08-06.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

IDemux *FFPlayerBuilder::CreateDemux() {
    IDemux *demux = new FFDemux();
    return demux;
}

IDecode *FFPlayerBuilder::CreateDecode() {
    IDecode * decode = new FFDecode();
    return decode;
}

IResample *FFPlayerBuilder::CreateResample() {
    IResample* resample = new FFResample();
    return resample;
}

IVideoView *FFPlayerBuilder::CreateVideoView() {
    IVideoView* iVideoView = new GLVideoView();
    return iVideoView;
}

IAudioPlay *FFPlayerBuilder::CreateAudioPlay() {
    IAudioPlay* audioPlay = new SLAudioPlay();
    return audioPlay;
}

IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index) {
    return  IPlayer::Get(index);
}
//PlayerBuilder 肯定要跟  FF player 这些耦合的
void FFPlayerBuilder::InitHard(void *vm) {
    FFDecode::InitHard(vm);
}
