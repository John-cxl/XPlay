//
// Created by John on 2023-07-12.
//

extern "C"
{
    #include <libavcodec/avcodec.h>
}

#include "FFDecode.h"

bool FFDecode::Open(XParameter para) {

    if(!para.para) return false;
    AVCodeParameters* p = para.para;

    return true;
}
