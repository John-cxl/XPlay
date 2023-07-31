//
// Created by John on 2023-08-01.
//

extern "C"
{
#include <libswresample/swresample.h>
}
#include <libavcodec//avcodec.h>
#include "FFResample.h"
#include "XLog.h"


bool FFResample::open(XParameter in, XParameter out) {

    //if(in.para->format)
    m_actx = swr_alloc();
    m_actx = swr_alloc_set_opts(m_actx,
                                av_get_default_channel_layout(2),
                                (AVSampleFormat)in.para->format, in.para->sample_rate,
                                av_get_default_channel_layout(in.para->channels),
                                (AVSampleFormat)in.para->format, in.para->sample_rate,
                                0,0);

    int re = swr_init(m_actx);
    if(0 != re)
    {
        XLOGD("swr_init failed");
        return false;
    }else
    {
        XLOGD("swr_init suceess");
    }

    return false;
}
