//
// Created by John on 2023-08-01.
//

extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

#include "FFResample.h"
#include "XLog.h"


bool FFResample::open(XParameter in, XParameter out) {

    //if(in.para->format)
    m_actx = swr_alloc();
    XLOGE("open start");
    m_actx = swr_alloc_set_opts(m_actx,
                                av_get_default_channel_layout(out.channels),
                                AV_SAMPLE_FMT_S16, out.sample_rate,
                                av_get_default_channel_layout(in.para->channels),
                                (AVSampleFormat)in.para->format, in.para->sample_rate,
                                0,0);
    XLOGE(" swr_alloc_set_opts");
    int re = swr_init(m_actx);
    if(0 != re)
    {
        XLOGD("swr_init failed");
        return false;
    }else
    {
        XLOGD("swr_init suceess");
    }
    m_outChannels = in.para->channels;
    m_outFormat   = AV_SAMPLE_FMT_S16;
    return true;
}

XData FFResample::Resample(XData indata) {
    //XLOGE("data size = %d", indata.size);
    if (indata.size <= 0 || !indata.pData)
    {
        return XData();
    }
    if(!m_actx) return XData();
    AVFrame *frame = (AVFrame *) indata.pData;
    //输出空间的分配
    XData out;

    //size = 通道数 * 单通道样本数 * 样被大小（样本字节大小）
    int outsize = m_outChannels * frame->nb_samples *av_get_bytes_per_sample((AVSampleFormat) m_outFormat);
    if (outsize <= 0)return XData();

    out.Alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.pData;
    int len = swr_convert(m_actx, outArr, frame->nb_samples, (const uint8_t **)frame->data,frame->nb_samples);
    if (len <= 0)
    {
        out.Drop();
        return XData();
    }
    //out.isAudio = 1;
   // XLOGE("swr_convert success = %d", len);

    return out;
}
