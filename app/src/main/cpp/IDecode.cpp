//
// Created by John on 2023-07-12.
//

#include <unistd.h>
#include "IDecode.h"
#include "XLog.h"
//观察者 被动收到 服务者的信息 被注册为 其他服务的观察者就可以接受到
void IDecode::Update(XData pkt) {

    if(pkt.isAudio != m_isAudio) //这里 只处理 自己对应的数据 音频和视频分离
    {
        return;
    }
    //XLOGE(" isAudio = %d is audio = %d", pkt.isAudio, m_isAudio);
    while(!isExit)
    {
        m_mutex.lock();
        if(m_maxDataNumber > m_listAudioData.size())//只有小于才会加 这样阻塞主以后 上面 的主题通知观察者也会阻塞住  不在下数据祈祷缓冲作用
        {
            m_listAudioData.push_back(pkt);
            m_mutex.unlock();
            break;
        }
        m_mutex.unlock();
        XSleep(1);
    }

}
void IDecode::Clear()
{
    m_mutex.lock();
    while(!m_listAudioData.empty())
    {
        m_listAudioData.front().Drop();
        m_listAudioData.pop_front();
    }
    m_curPts = 0;
    synPts = 0;
    m_mutex.unlock();
}

//这里是消费者 在消费
void IDecode::Main()
{
    while(!isExit)
    {
        m_mutex.lock();
        //XLOGD("MAIN 111111");
        //判断音视频同步  在视频 code 中进行判断
        if(!m_isAudio && synPts > 0)
        {
            if(synPts < m_curPts)
            {
                m_mutex.unlock();
                XSleep(2);
                continue;
            }
        }

        if(m_listAudioData.empty()) //如果是空的 那么接解锁 睡一下继续循环   这里是
        {
            m_mutex.unlock();
            XSleep(2);
            continue;
        }
        //XLOGD("MAIN 222222");
        //取出数据
        XData data = m_listAudioData.front(); //取出就从 队列中移除
        m_listAudioData.pop_front();
        //发送数据到解码器线程 一个数据包可能解码多个 数据
        if(this->SendPacket(data))
        {
            //接受解析后的数据
           while(!isExit)
           {
               XData frame =  ReceiveFrame();
               if(!frame.pData)break;
               m_curPts = frame.pts;
               this->Notify(frame);
           }
        }
        data.Drop();   //这里会内存泄露
        m_mutex.unlock();
    }
}


