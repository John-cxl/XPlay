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
    //XLOGD("PLAY CALL");
    XData d = GetData(); //阻塞的函数
    if(d.size <=0)
    {
        XLOGE("GetData() size is 0");
        return ;
    }
    if(!m_buf)return ;

    memcpy(m_buf, d.pData, d.size);

    m_mux.lock();
    (*pbf)->Enqueue(pbf,m_buf, d.size);
    m_mux.unlock();

    d.Drop();
}

static void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *contex)
{
    SLAudioPlay *ap = (SLAudioPlay*)contex;
    //XLOGD("PcmCall sssssss");
    if(!ap)
    {
        XLOGE("PcmCall ap is null");
        return;
    }
    ap->PlayCall((void *)bf);
}

void SLAudioPlay::Closer()
{
    m_mux.lock();
    //停止播放
    if(g_iplayer && (*g_iplayer))
    {
        (*g_iplayer)->SetPlayState(g_iplayer, SL_PLAYSTATE_STOPPED);
    }
    //清理播放队列
    if(g_pcmQue && (*g_pcmQue))
    {
        (*g_pcmQue)->Clear(g_pcmQue);
    }

    //销毁player 对象
    if(g_player && (*g_player))
    {
        (*g_player)->Destroy(g_player);
    }

    //销毁混音器
    if(g_mix && (*g_mix))
    {
        (*g_mix)->Destroy(g_mix);
    }

    //销毁播放引擎
    if(g_engineSL && (*g_engineSL))
    {
        (*g_engineSL)->Destroy(g_engineSL);
        g_eng = nullptr;
    }
    m_mux.unlock();
}

bool SLAudioPlay::StartPlay(XParameter out)
{
    Closer();
    //1创建引擎
    m_mux.lock();
    g_eng = CreateSl();
    if(g_eng)
    {
        XLOGD("CreateSL success!!");
    }else
    {
        m_mux.unlock();
        XLOGD("CreateSL Failed!!");
        return false;
    }

    //2 创建混音器
    SLresult re = 0;
    re = (*g_eng)->CreateOutputMix(g_eng, &g_mix, 0, 0, 0);
    if(re != SL_RESULT_SUCCESS)
    {
        m_mux.unlock();
        XLOGE("CreateOutputMix Failed");
        return false;
    }
    re = (*g_mix)->Realize(g_mix, SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS)
    {
        m_mux.unlock();
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
    SLDataFormat_PCM pcm;
    if(out.channels == 2)
    {
        pcm={
                SL_DATAFORMAT_PCM,
                (SLuint32)out.channels,//声道数 必须和 下面 和mask  想匹配
                (SLuint32)out.sample_rate*1000,
                SL_PCMSAMPLEFORMAT_FIXED_16,
                SL_PCMSAMPLEFORMAT_FIXED_16,
                SL_SPEAKER_FRONT_LEFT| SL_SPEAKER_FRONT_RIGHT,//| SL_SPEAKER_FRONT_CENTER| SL_SPEAKER_LOW_FREQUENCY | SL_SPEAKER_BACK_LEFT | SL_SPEAKER_BACK_RIGHT,//这里必须设置这么多不然 声道太多会创建失败
                SL_BYTEORDER_LITTLEENDIAN//字节序 小端
        };
    }
    else if(out.channels == 6)
    {
        pcm={
                SL_DATAFORMAT_PCM,
                (SLuint32)out.channels,//声道数 必须和 下面 和mask  想匹配
                (SLuint32)out.sample_rate*1000,
                SL_PCMSAMPLEFORMAT_FIXED_16,
                SL_PCMSAMPLEFORMAT_FIXED_16,
                SL_SPEAKER_FRONT_LEFT| SL_SPEAKER_FRONT_RIGHT| SL_SPEAKER_FRONT_CENTER| SL_SPEAKER_LOW_FREQUENCY | SL_SPEAKER_BACK_LEFT | SL_SPEAKER_BACK_RIGHT,//这里必须设置这么多不然 声道太多会创建失败
                SL_BYTEORDER_LITTLEENDIAN//字节序 小端
        };
    }

    SLDataSource  ds = {&que, &pcm};

     //4 创建播放器
     const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
     const SLboolean req[] = {SL_BOOLEAN_TRUE};

     re = (*g_eng)->CreateAudioPlayer(g_eng, &g_player, &ds, &audioSink, sizeof(ids)/sizeof(SLInterfaceID), ids,req);
     if(re != SL_RESULT_SUCCESS){
         m_mux.unlock();
         XLOGE("CreateAudioPlayer Failed re = %d ", re);
         return false;
     }else{
         XLOGE("CreateAudioPlayer Success");
     }

    (*g_player)->Realize(g_player, SL_BOOLEAN_FALSE);
     //获取 player 接口
    re = (*g_player)->GetInterface(g_player, SL_IID_PLAY, &g_iplayer);
    if(re != SL_RESULT_SUCCESS){
        m_mux.unlock();
        XLOGE("GetInterface SL_IID_PLAY Failed");
        return false;
    }
    re = (*g_player)->GetInterface(g_player,SL_IID_BUFFERQUEUE, &g_pcmQue);
    XLOGD("re = %d ", re);
    if(re != SL_RESULT_SUCCESS){
        m_mux.unlock();
        XLOGE("GetInterface 2  Failed");
        return false;
    }
    //设置回调函数， 播放队列 调用
    re = (*g_pcmQue)->RegisterCallback(g_pcmQue, PcmCall, this);
    if(re != SL_RESULT_SUCCESS)
    {
        XLOGE("RegisterCallback is failed");
    }

    //设置为播放状态
    (*g_iplayer)->SetPlayState(g_iplayer, SL_PLAYSTATE_PLAYING);
    //启动列队 回调
    (*g_pcmQue)->Enqueue(g_pcmQue, "", 1);
    XLOGE("SLAudioPlay StartPlay  success");
    m_mux.unlock();
    return true;
}

SLAudioPlay::SLAudioPlay() {
    m_buf = new unsigned char[1024*1024];
}

SLAudioPlay::~SLAudioPlay() {
    delete m_buf;
    m_buf = 0;
}




