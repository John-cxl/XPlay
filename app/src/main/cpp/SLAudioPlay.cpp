//
// Created by John on 2023-08-03.
//
#include "SLAudioPlay.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "XLog.h"

static SLObjectItf g_engineSL = NULL;
static SLEngineItf g_eng = NULL;
static SLObjectItf g_mix = NULL;
static SLObjectItf g_player = NULL;
static SLPlayItf  g_iplayer = NULL;
static SLAndroidSimpleBufferQueueItf g_pcmQue = NULL;

static SLEngineItf CreateSl()
{
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&g_engineSL, 0, 0,0,0,0);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*g_engineSL)->Realize(g_engineSL, SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*g_engineSL)->GetInterface(g_engineSL, SL_IID_ENGINE, &en);
    if(re != SL_RESULT_SUCCESS) return NULL;
    return en;
}

void SLAudioPlay::PlayCall(void *bufq)
{
    if(!bufq)return ;
    SLAndroidSimpleBufferQueueItf pbf = (SLAndroidSimpleBufferQueueItf)bufq;
    XLOGD("PLAY CALL");
}

static void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *contex)
{
    SLAudioPlay *ap = (SLAudioPlay*)contex;
    if(!ap)
    {
        XLOGE("PcmCall ap is null");
        return;
    }
    ap->PlayCall((void *)bf);
}

bool SLAudioPlay::StartPlay(XParameter out)
{
    //1创建引擎
    g_eng = CreateSl();
    if(g_eng)
    {
        XLOGD("CreateSL success!!");
    }else
    {
        XLOGD("CreateSL Failed!!");
        return false;
    }

    //2 创建混音器
    SLresult re = 0;
    re = (*g_eng)->CreateOutputMix(g_eng, &g_mix, 0, 0, 0);
    if(re != SL_RESULT_SUCCESS)
    {
        XLOGE("CreateOutputMix Failed");
        return false;
    }
    re = (*g_mix)->Realize(g_mix, SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS)
    {
        XLOGE("Realize Failed");
        return false;
    }
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, g_mix};
    SLDataSink  audioSink={&outputMix, 0};

    //3 配置音频信息
    //缓冲队列
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    XLOGD("chnnnles = %d rate = %d ", out.channels, out.sample_rate);
    SLDataFormat_PCM pcm={
            SL_DATAFORMAT_PCM,
            (SLuint32)out.channels,//声道数 必须和 下面 和mask  想匹配
            (SLuint32)out.sample_rate*1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT| SL_SPEAKER_FRONT_RIGHT| SL_SPEAKER_FRONT_CENTER
                                   | SL_SPEAKER_LOW_FREQUENCY | SL_SPEAKER_BACK_LEFT | SL_SPEAKER_BACK_RIGHT,//这里必须设置这么多不然 声道太多会创建失败
            SL_BYTEORDER_LITTLEENDIAN//字节序 小端
    };
    SLDataSource  ds = {&que, &pcm};

     //4 创建播放器
     const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
     const SLboolean req[] = {SL_BOOLEAN_TRUE};

     re = (*g_eng)->CreateAudioPlayer(g_eng, &g_player, &ds, &audioSink, sizeof(ids)/sizeof(SLInterfaceID), ids,req);
     if(re != SL_RESULT_SUCCESS){

         XLOGE("CreateAudioPlayer Failed re = %d ", re);
         return false;
     }else{
         XLOGE("CreateAudioPlayer Success");
     }

    (*g_player)->Realize(g_player, SL_BOOLEAN_FALSE);
     //获取 player 接口
    re = (*g_player)->GetInterface(g_player, SL_IID_PLAY, &g_iplayer);
    if(re != SL_RESULT_SUCCESS){
        XLOGE("GetInterface SL_IID_PLAY Failed");
        return false;
    }
    re = (*g_player)->GetInterface(g_player,SL_IID_BUFFERQUEUE, &g_pcmQue);
    if(re != SL_RESULT_SUCCESS){
        XLOGE("GetInterface 2  Failed");
        return false;
    }
    //设置回调函数， 播放队列 调用
    (*g_pcmQue)->RegisterCallback(g_pcmQue, PcmCall, this);

    //设置为播放状态
    (*g_iplayer)->SetPlayState(g_iplayer, SL_PLAYSTATE_PLAYING);
    //启动列队 回调
    (*g_pcmQue)->Enqueue(g_pcmQue, "", 1);
    XLOGE("SLAudioPlay StartPlay  success");
    return true;
}


