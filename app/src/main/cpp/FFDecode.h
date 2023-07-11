//
// Created by John on 2023-07-12.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "IDecode.h"

class FFDecode : public IDecode{
public:
    virtual bool Open(XParameter para) ;
};


#endif //XPLAY_FFDECODE_H
