//
// Created by John on 2023-07-11.
//

#include "IObserver.h"
#include "XLog.h"

void IObserver::Notify(XData data) {
    m_mutex.lock();
    for(int i = 0; i < m_vector.size(); i++)
    {
        //XLOGD(" notify ");
        m_vector[i]->Updata(data);
    }
    m_mutex.unlock();
}

void IObserver::AddObserver(IObserver *pObserver) {
    m_mutex.lock();
    m_vector.push_back(pObserver);
    m_mutex.unlock();
}
