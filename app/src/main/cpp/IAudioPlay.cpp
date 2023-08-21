//
// Created by John on 2023-08-03.
//

#include "IAudioPlay.h"
#include "XLog.h"

XData IAudioPlay::GetData() {

    XData d;
    isRunning = true;
    while(!isExit)
    {
        if(isPause)
        {
            XSleep(2);
            continue;
        }

        m_mutexAudio.lock();
        if(!m_listData.empty())
        {
            d = m_listData.front();
            m_listData.pop_front();
            m_mutexAudio.unlock();
            m_pts = d.pts;
            return d;
        }
        m_mutexAudio.unlock();
        XSleep(1);
    }
    //m_pts = 0;
    //异常的获取
    isRunning = false;
    return d;
}

void IAudioPlay::Clear()
{
    m_mutexAudio.lock();
    while(!m_listData.empty())
    {
        m_listData.front().Drop();
        m_listData.pop_front();
    }
    m_mutexAudio.unlock();
}

void IAudioPlay::Update(XData data) {

    //XLOGD("data size = %d isAudio %d", data.size, data.isAudio);
    if(data.size < 0 || !data.pData)return;
    //压入缓冲队列 音频一定要 做好缓冲
    while(!isExit)
    {
        m_mutexAudio.lock();
        if(m_listData.size() > m_iMaxDataNum)
        {
            m_mutexAudio.unlock();
            XSleep(1); // 睡一下不然CPU 资源占完了
            continue;
        }
        //XLOGD("push back ");
        m_listData.push_back(data);
        m_mutexAudio.unlock();
        break;
    }
}



