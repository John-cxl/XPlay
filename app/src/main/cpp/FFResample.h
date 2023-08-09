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
    virtual void Close();
    virtual XData Resample(XData indata);
protected:
    SwrContext *m_actx;
    std::mutex m_mux;
};


#endif //XPLAY_FFRESAMPLE_H
