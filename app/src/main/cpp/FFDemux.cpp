//
// Created by John on 2023-07-05.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"  //这个引用的是 C语言写的
{
    #include <libavformat/avformat.h>
};
//分数转为浮点数
static double r2d(AVRational r)
{
    return r. num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}


FFDemux::FFDemux() {
    static bool bFirst = true;
    if (bFirst)
    {
        bFirst = false;
        //注册所有的 封装器
        av_register_all();
        //注册所有的 解封装器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
    }
}

bool FFDemux::Seek(double pos)
{
    if(pos < 0 || pos >1)
    {
        XLOGD("ffdemux seek failed pos must 0.0 - 1.0");
        return false;
    }
    bool b = false;
    mux.lock();
    if(!m_pIc)
    {
        mux.unlock();
        return false;
    }
    //清理读取的缓冲
    avformat_flush(m_pIc);
    long long seekPts = 0;
    seekPts = m_pIc->streams[m_videoStreamIndex]->duration * pos;
    //往后跳转到关键帧
    b = av_seek_frame(m_pIc, m_videoStreamIndex, seekPts,AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return b;

}
//定义打开的接口
bool FFDemux::open(const char* url)
{
    XLOGI("open file %s begin\n", url);
    Close();

    mux.lock();

    int ret = avformat_open_input(&m_pIc, url, 0, 0); //这个是打开
    if(ret != 0)
    {
        mux.unlock();
        char buf[1024]= {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("open rul %s failed error %s ", url, buf);
        return false;
    }

    XLOGI("open rul %s success", url);

    ret = avformat_find_stream_info(m_pIc,0);
    if(ret != 0)
    {
        mux.unlock();
        char buf[1024]= {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("find info failed  error %s ", buf);
        return false;
    }

    XLOGI("find info success");

    m_totalMs = m_pIc->duration / (AV_TIME_BASE / 1000);

    XLOGI("find info success total ms = %ld ", m_totalMs);
    mux.unlock(); //提前解锁下面 也有锁会死锁
    GetVPara();
    GetAPara();

    return true;
}


XData FFDemux::read()
{
    mux.lock();

    if(!m_pIc)
    {
        mux.unlock();
        return XData();
    }

    XData d;
    AVPacket* pPacket = av_packet_alloc(); //申请空间

    int ret = av_read_frame(m_pIc, pPacket);  //读取信息到包里面
    if(ret != 0)
    {
        av_packet_free(&pPacket);
        mux.unlock();
        return XData();
    }

    d.pData = (unsigned char*)pPacket;
    d.size = pPacket->size;

    if(pPacket->stream_index == m_videoStreamIndex)
    {
        d.isAudio = false;
    }
    else if(pPacket->stream_index == m_audioStreamIndex)
    {
        d.isAudio = true;
    }
    else
    {
        XLOGE(" it is not video and audio");
        mux.unlock();
        av_packet_free(&pPacket);
        return XData();
    }

    //XLOGI("size = %d, pts = %lld isAudio = %d",pPacket->size, pPacket->pts, d.isAudio);
    //转换 pts
    pPacket->pts = pPacket->pts *1000* r2d(m_pIc->streams[pPacket->stream_index]->time_base);
    pPacket->dts = pPacket->dts *1000* r2d(m_pIc->streams[pPacket->stream_index]->time_base);
    d.pts = (int)pPacket->pts;
    //XLOGI("d.tps = %d", d.pts);
    mux.unlock();
    return d;
}


XParameter FFDemux::GetVPara() {

    mux.lock();
    if(!m_pIc)
    {
        mux.unlock();
        XLOGE(" ic is le  null pointer");
        return XParameter();
    }

    int ret = av_find_best_stream(m_pIc, AVMEDIA_TYPE_VIDEO, -1,-1, 0,0);
    if(ret < 0)
    {
        mux.unlock();
        XLOGE("av_find_best_stream error  result is [%d]", ret);
        return XParameter();
    }
    XLOGD("av_find_best_stream success");
    m_videoStreamIndex = ret;
    XParameter para;
    para.para = m_pIc->streams[ret]->codecpar;
    mux.unlock();
    return para;
}

XParameter FFDemux::GetAPara() {
    mux.lock();
    if(!m_pIc)
    {
        XLOGE(" ic is le  null pointer");
        mux.unlock();
        return XParameter();
    }

    //这里是返回的是流的索引
    int ret = av_find_best_stream(m_pIc, AVMEDIA_TYPE_AUDIO, -1,-1, 0,0);
    if(ret < 0)
    {
        XLOGE("av_find_best_stream error  result is [%d]", ret);
        mux.unlock();
        return XParameter();
    }
    m_audioStreamIndex = ret;
    XParameter para;
    para.para = m_pIc->streams[ret]->codecpar;
    para.channels = m_pIc->streams[ret]->codecpar->channels;
    para.sample_rate = m_pIc->streams[ret]->codecpar->sample_rate;
    mux.unlock();
    return para;
}

void FFDemux::Close() {
    mux.lock();
    if(m_pIc)
        avformat_close_input(&m_pIc);
    mux.unlock();
}








