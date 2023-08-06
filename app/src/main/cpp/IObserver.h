//
// Created by John on 2023-07-11.
//

#ifndef XPLAY_IOBSERVER_H
#define XPLAY_IOBSERVER_H


#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>


class IObserver :public XThread{
public :
    //自己又是主题 又是观察者  这个有的需要实现有的不需要，所以我们实现一个空的
    virtual void Update(XData data){};
    //增加观察者
    void AddObserver(IObserver* pObserver);
    //作为主体去通知 观察者 调用 updata 观察者只要实现了这个接口就会接到通知
    void Notify(XData data);
private:
    std::vector<IObserver* > m_vector;
    std::mutex               m_mutex;

};


#endif //XPLAY_IOBSERVER_H
