//
// Created by John on 2023-08-01.
//

#ifndef XPLAY_FFRESAMPLE_H
#define XPLAY_FFRESAMPLE_H


#include "IResample.h"
class SwrContext;

class FFResample: public IResample{
public:
    virtual bool open(XParameter in, XParameter out = XParameter());

protected:
    SwrContext *m_actx;
};


#endif //XPLAY_FFRESAMPLE_H
