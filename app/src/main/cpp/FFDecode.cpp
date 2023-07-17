//
// Created by John on 2023-07-12.
//

extern "C"
{
#include <libavcodec/avcodec.h>
}

#include "FFDecode.h"
#include "XLog.h"

bool FFDecode::Open(XParameter para) {

    if(!para.para) return false;
    AVCodecParameters* p = para.para;
    //查找解码器 通过id
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if(!cd)
    {
        XLOGE("avcodec_find_decoder failed");
        return false;
    }
    //创建解码上下文
    m_pCodecContext = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(m_pCodecContext, p);

    m_pCodecContext->thread_count = 8; //设置好多线程 解码
    //打开解码器 后面两个是配置参数 暂时用不到
    int ret = avcodec_open2(m_pCodecContext, 0, 0);
    if(0 != ret)
    {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        XLOGE("error = %s", buf);
        return false;
    }

    if(m_pCodecContext->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        this->m_isAudio = false;
    }
    else
    {
        this->m_isAudio = true;
    }

    XLOGI("open avcodec open success");

    return true;
}

bool FFDecode::SendPacket(XData pkt) {
    if(!pkt.pData || 0 >= pkt.size){return false;}

    //上一步的 解码器上下文没有获取到
    if(!m_pCodecContext)
    {
        return false;
    }

    int ret = avcodec_send_packet(m_pCodecContext, (AVPacket*)pkt.pData); //这里是立即返回的
    if(0 != ret)
    {
        XLOGE("avcodec_send_frame failed\n");
        return false;
    }

    return true;
}

XData FFDecode::ReceiveFrame() {

    if(!m_pCodecContext)
    {
        return XData();
    }
    if(!m_pFrame)
    {
        m_pFrame = av_frame_alloc();
    }
    int ret = avcodec_receive_frame(m_pCodecContext,m_pFrame);
    if(0 != ret)
    {
        //XLOGE("avcodec_receive_frame failed\n");
        return XData();
    }
    XData data;
    data.pData = (unsigned char*)m_pFrame;
    if(AVMEDIA_TYPE_VIDEO == m_pCodecContext->codec_type)
    {
        data.isAudio = false;
        data.size = (m_pFrame->linesize[0] + m_pFrame->linesize[1] +m_pFrame->linesize[2]) * m_pFrame->height; //视频的 结构定义的
    }
    else
    {
        //样本字节数 * 单通道样本数 * 2通道
        data.isAudio = true;
        data.size = av_get_bytes_per_sample((AVSampleFormat)m_pFrame->format) * m_pFrame->nb_samples * 2;
    }

    return data;
}
