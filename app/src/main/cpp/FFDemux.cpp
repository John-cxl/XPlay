//
// Created by John on 2023-07-05.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"  //这个引用的是 C语言写的
{
    #include <libavformat/avformat.h>
};



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


//定义打开的接口
bool FFDemux::open(const char* url)
{
    XLOGI("open file %s begin\n", url);
    int ret = avformat_open_input(&m_pIc, url, 0, 0); //这个是打开
    if(ret != 0)
    {
        char buf[1024]= {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("open rul %s failed error %s ", url, buf);
        return false;
    }

    XLOGI("open rul %s success", url);

    ret = avformat_find_stream_info(m_pIc,0);
    if(ret != 0)
    {
        char buf[1024]= {0};
        av_strerror(ret, buf, sizeof(buf));
        XLOGE("find info failed  error %s ", buf);
        return false;
    }

    XLOGI("find info success");

    m_totalMs = m_pIc->duration / (AV_TIME_BASE / 1000);

    XLOGI("find info success total ms = %ld ", m_totalMs);
    GetVPara();
    GetAPara();
    return true;
}


XData FFDemux::read()
{
    if(!m_pIc)return XData();

    XData d;
    AVPacket* pPacket = av_packet_alloc(); //申请空间

    int ret = av_read_frame(m_pIc, pPacket);  //读取信息到包里面
    if(ret != 0)
    {
        av_packet_free(&pPacket);
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
        av_packet_free(&pPacket);
        return XData();
    }

    //XLOGI("size = %d, pts = %lld isAudio = %d",pPacket->size, pPacket->pts, d.isAudio);

    return d;
}


XParameter FFDemux::GetVPara() {

    if(!m_pIc)
    {
        XLOGE(" ic is le  null pointer");
        return XParameter();
    }

    int ret = av_find_best_stream(m_pIc, AVMEDIA_TYPE_VIDEO, -1,-1, 0,0);
    if(ret < 0)
    {
        XLOGE("av_find_best_stream error  result is [%d]", ret);
        return XParameter();
    }
    XLOGD("av_find_best_stream success");
    m_videoStreamIndex = ret;
    XParameter para;
    para.para = m_pIc->streams[ret]->codecpar;
    return para;
}

XParameter FFDemux::GetAPara() {
    if(!m_pIc)
    {
        XLOGE(" ic is le  null pointer");
        return XParameter();
    }

    //这里是返回的是流的索引
    int ret = av_find_best_stream(m_pIc, AVMEDIA_TYPE_AUDIO, -1,-1, 0,0);
    if(ret < 0)
    {
        XLOGE("av_find_best_stream error  result is [%d]", ret);
        return XParameter();
    }
    m_audioStreamIndex = ret;
    XParameter para;
    para.para = m_pIc->streams[ret]->codecpar;
    return para;
}


//定义关闭的接口
XData FFDemux::close()
{
    XData a;
    return a;
}








