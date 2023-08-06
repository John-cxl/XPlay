//
// Created by John on 2023-08-03.
//

#ifndef XPLAY_IAUDIOPLAY_H
#define XPLAY_IAUDIOPLAY_H


#include <list>
#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay: public IObserver{
public:
    //阻塞的 函数 缓冲数据满了以后会阻塞
    virtual void Update(XData data);
    //获取缓冲数据， 如没有则组赛
    virtual XData GetData();
    virtual bool StartPlay(XParameter out) = 0;
    int m_iMaxDataNum;
protected:
    std::list<XData> m_listData;
    std::mutex       m_mutexAudio;

};


#endif //XPLAY_IAUDIOPLAY_H
