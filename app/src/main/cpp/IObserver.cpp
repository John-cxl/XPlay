//
// Created by John on 2023-07-11.
//

#include "IObserver.h"
#include "XLog.h"

void IObserver::Notify(XData data) {
    m_mutex.lock();
    for(int i = 0; i < m_vector.size(); i++)
    {
//        XLOGD(" notify i = %d", i);
        m_vector[i]->Update(data);
    }
    m_mutex.unlock();
}

void IObserver::AddObserver(IObserver *pObserver) {

    //XLOGD("add Observer111111");
    if(!pObserver)return ;
    m_mutex.lock();
    //XLOGD("add Observer222222");
    m_vector.push_back(pObserver);
    m_mutex.unlock();
}
