//
// Created by John on 2023-07-18.
//

#include "XEGL.h"
//基类是纯虚函数 利用他继承 并初始化 Init 初始化窗口操作 并用 XEGL get 中设置CXEGL 来实现单例模式
class CXEGL:public XEGL
{
public:
    virtual bool Init(void *win) override
    {
        return true;
    }
};


XEGL *XEGL::Get() {
    static CXEGL egl; //实现单例模式
    return &egl;
}
