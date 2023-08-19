//
// Created by John on 2023-07-18.
//
#include <android/native_window.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "XLog.h"
#include "XEGL.h"
#include <mutex>
//基类是纯虚函数 利用他继承 并初始化 Init 初始化窗口操作 并用 XEGL get 中设置CXEGL 来实现单例模式
class CXEGL:public XEGL
{
public:
    EGLDisplay  m_display = EGL_NO_DISPLAY;
    EGLSurface  m_surface = EGL_NO_SURFACE;
    EGLContext  m_context = EGL_NO_CONTEXT;
    std::mutex  m_mutex;
    virtual void Draw()
    {
        m_mutex.lock();
        if(EGL_NO_DISPLAY == m_display || EGL_NO_SURFACE == m_surface)
        {
            m_mutex.unlock();
            XLOGE("EGL_NO_DISPLAY == m_display || EGL_NO_SURFACE == m_surface");
            return;
        }

        EGLBoolean  b = eglSwapBuffers(m_display , m_surface);
        m_mutex.unlock();
        //XLOGD( " eglSwapBuffers is %d", b);
    }

    virtual void Close()
    {
        m_mutex.lock();
        if(m_display == EGL_NO_DISPLAY)
        {
            m_mutex.unlock();
            return;
        }
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if(m_surface != EGL_NO_SURFACE)
            eglDestroySurface(m_display, m_surface);
        if(m_context != EGL_NO_CONTEXT)
            eglDestroyContext(m_display, m_context);
        m_display = EGL_NO_DISPLAY;
        m_surface = EGL_NO_SURFACE;
        m_context = EGL_NO_CONTEXT;
        m_mutex.unlock();
        return;
    }

    virtual bool Init(void *win) override
    {

        ANativeWindow *pWin = (ANativeWindow*)win;
        Close();
        //初始化EGL
        //获取EGLDisplay对象 显示设备
        m_mutex.lock();
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(EGL_NO_DISPLAY == m_display)
        {
            m_mutex.unlock();
            XLOGE("eglGetDisplay failed");
            return false;
        }
        XLOGD("eglGetDisplay success!");

        //2初始化Display
        if(EGL_TRUE != eglInitialize(m_display,0,0))
        {
            m_mutex.unlock();
            XLOGE("eglInitialize failed");
            return false;
        }
        XLOGD("eglInitialize success!");

        //3获取配置并创建 surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };

        EGLint const static attribs[] = {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_NATIVE_RENDERABLE, EGL_TRUE,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR, // new requirement
                EGL_NONE
        };

        //3.1获取配置
        EGLConfig config = 0;
        EGLint    numConfigs = 0;
        if(EGL_TRUE != eglChooseConfig(m_display, configSpec, &config, 1, &numConfigs))
        {
            m_mutex.unlock();
            XLOGE("eglChooseConfig failed");
            return false;
        }
        XLOGD("eglChooseConfig success!");

        //3.2创建 surface
        m_surface = eglCreateWindowSurface(m_display, config,pWin, NULL);

        //4创建并打开EGL 上下文
        const EGLint  ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
        };
        m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, ctxAttr);
        if(EGL_NO_CONTEXT == m_context)
        {
            m_mutex.unlock();
            XLOGE("eglCreateContext failed");
            return false;
        }

        XLOGD("eglCreateContext success");

       if(EGL_TRUE !=  eglMakeCurrent(m_display, m_surface,m_surface,m_context))
       {
           m_mutex.unlock();
           XLOGE("eglMakeCurrent failed");
           return false;
       }
        m_mutex.unlock();
        XLOGD("eglMakeCurrent success");
        return true;
    }

};


XEGL *XEGL::Get() {
    static CXEGL egl; //实现单例模式
    return &egl;
}
