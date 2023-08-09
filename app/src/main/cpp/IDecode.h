//
// Created by John on 2023-07-12.
//

#ifndef XPLAY_IDECODE_H
#define XPLAY_IDECODE_H

#include "XParameter.h"
#include "IObserver.h"
#include <list>
//解码接口， 支持硬解码
class IDecode :public IObserver{
public:

    //打开解码器
    virtual bool Open(XParameter para, bool isHard = false) = 0;
    virtual bool SendPacket(XData pkt) = 0;   //发送数据
    virtual void Close() = 0;
    //再次调用 会复用上次 空间 线程不安全
    virtual XData ReceiveFrame() = 0;
    virtual void Update(XData data);          //更新数据 观察者 会阻塞到这里
    bool    m_isAudio = false;
    int     m_maxDataNumber = 100;   //不能无限制的存下去 不然的话 不从队列中取数据 内存会耗尽

    //同步时间，再次打开文件要清理
    int synPts = 0;
    int m_curPts = 0;
private:
    virtual void Main();
    std::list<XData> m_listAudioData;
    std::mutex       m_mutex; //用于 读取数据 加锁
};


#endif //XPLAY_IDECODE_H
