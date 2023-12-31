//
// Created by John on 2023-08-03.
//

#ifndef XPLAY_SLAUDIOPLAY_H
#define XPLAY_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay: public IAudioPlay {
public:
    virtual bool StartPlay(XParameter out);
    void PlayCall(void *bufq);
    virtual void Closer();
    SLAudioPlay();
    virtual ~SLAudioPlay();
protected:
    unsigned char *m_buf = 0;
    std::mutex m_mux;
};


#endif //XPLAY_SLAUDIOPLAY_H
