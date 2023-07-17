//
// Created by John on 2023-07-12.
//

#include <unistd.h>
#include "IDecode.h"
#include "XLog.h"

void IDecode::Updata(XData pkt) {

    if(pkt.isAudio != m_isAudio)
    {
        return;
    }

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

//这里是消费者 在消费
void IDecode::Main()
{
    while(!isExit)
    {
        m_mutex.lock();

        if(m_listAudioData.empty()) //如果是空的 那么接解锁 睡一下继续循环   这里是
        {
            m_mutex.unlock();
            XSleep(1);
            continue;
        }
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

               XLOGE("receive frame size = %d %d", frame.size, frame.isAudio);
               this->Notify(frame);
           }
        }
        data.Drop();   //这里会内存泄露
        m_mutex.unlock();
    }
}
