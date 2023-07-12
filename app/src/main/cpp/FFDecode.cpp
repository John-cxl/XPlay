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
    }
    //创建解码上下文
    m_pCodecContext = avcodec_alloc_context3(cd);
    int ret = avcodec_parameters_to_context(m_pCodecContext, p);

    //打开解码器 后面两个是配置参数 暂时用不到
    ret = avcodec_open2(m_pCodecContext, 0, 0);
    if(0 != ret)
    {
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        XLOGE("error = %s", buf);
        return false;
    }

    XLOGI("open avcodec open success");

    return true;
}
