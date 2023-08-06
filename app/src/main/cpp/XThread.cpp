//
// Created by John on 2023-07-09.
//

#include "XThread.h"
#include "XLog.h"
#include <thread>
using namespace std;


void XSleep(int mis)
{
    chrono::milliseconds  du(mis);
    this_thread::sleep_for(du);
}

bool XThread::Start()
{
    isExit = false;
    thread th(&XThread::ThreadMain, this);
    th.detach(); //放弃对新创建线程 控制
    return true;
}

void XThread::Stop()
{
    isExit = true;
    for(int i = 0; i < 200; i++)
    {
        if(!isRunning)
        {
            XLOGI("stop 停止线程成功");
            return;
        }
        XSleep(1);
    }
    XLOGI("stop thread outtime");

}

void XThread::ThreadMain()
{
    isRunning = true;
    XLOGI("线程进入");
    Main();
    XLOGI("线程退出");
    isRunning = false;
}