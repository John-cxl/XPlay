//
// Created by John on 2023-08-01.
//

#ifndef XPLAY_IRESAMPLE_H
#define XPLAY_IRESAMPLE_H


#include "XParameter.h"
#include "IObserver.h"

class IResample:public IObserver {
public:
    virtual bool open(XParameter in, XParameter out = XParameter())=0;
    virtual XData Resample(XData indata) = 0;
    virtual void Update(XData data);
};


#endif //XPLAY_IRESAMPLE_H
