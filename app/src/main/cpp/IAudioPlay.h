//
// Created by John on 2023-08-03.
//

#ifndef XPLAY_IAUDIOPLAY_H
#define XPLAY_IAUDIOPLAY_H


#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay: public IObserver{
public:
    virtual void Updata(XData data);
    virtual bool StartPlay(XParameter out) = 0;

};


#endif //XPLAY_IAUDIOPLAY_H
