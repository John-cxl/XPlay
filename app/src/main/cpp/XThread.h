//
// Created by John on 2023-07-09.
//

#ifndef XPLAY_XTHREAD_H
#define XPLAY_XTHREAD_H

//sleep 毫秒
void XSleep(int mis);

//c++11 线程库

class XThread
{
public:
    virtual void Start(); //启动线程
    virtual void Stop();  //安全停止线程 不一定成功 通过控制变量 isExit  不要直接停止，你不知道他进行到那一步了
    virtual void Main() {}  //入口主函数

protected:
    bool isExit = false;
    bool isRunning = true;
private:
    void ThreadMain();
};


#endif //XPLAY_XTHREAD_H
