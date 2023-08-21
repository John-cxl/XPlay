//
// Created by John on 2023-07-05.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main()
{
    while(!isExit)
    {
        if(isPause)
        {
            XSleep(2);
            continue;
        }
        XData data = read();
        if(0 < data.size)
        {
//            XLOGD("IDemux start notify ");
            Notify(data);  //作为主体告知 通知观察者并发送数据
        }else
        {
            XSleep(2); //读不到数据小睡一下
        }
//        XLOGI("size = %d", data.size);
//        if(0 >= data.size){break;}
    }
}