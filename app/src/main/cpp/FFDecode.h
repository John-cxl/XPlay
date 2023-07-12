//
// Created by John on 2023-07-12.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "IDecode.h"
struct AVCodecContext;

class FFDecode : public IDecode{
public:
    virtual bool Open(XParameter para) ;

protected:
    AVCodecContext * m_pCodecContext;
};


#endif //XPLAY_FFDECODE_H
