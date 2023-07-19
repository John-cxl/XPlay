//
// Created by John on 2023-07-18.
//

#ifndef XPLAY_XEGL_H
#define XPLAY_XEGL_H


class XEGL {
public:
    virtual bool Init(void * win) = 0; //写成纯虚函数 也写成单件模式
    static XEGL * Get();

protected:
    XEGL(){}
};


#endif //XPLAY_XEGL_H
