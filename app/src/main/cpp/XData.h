//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_XDATA_H
#define XPLAY_XDATA_H


struct XData {
    unsigned char * pData = 0;
    int size = 0;
    bool isAudio = false;
    void Drop();
};


#endif //XPLAY_XDATA_H
