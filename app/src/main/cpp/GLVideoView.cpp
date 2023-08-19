//
// Created by John on 2023-07-18.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "XLog.h"

void GLVideoView::SetRender(void * pWin)
{
    XLOGD(" SetRender");
    m_pView = pWin;
}

void GLVideoView::Close()
{
    m_mutex.lock();
    if(m_pTexture)
    {
        XLOGD("close");
        m_pTexture->Drop();
        m_pTexture = 0;
    }
    m_mutex.unlock();
}

void GLVideoView::Render(XData data)
{
    //XLOGD("start render1111111");
    if(!m_pView)
    {
        //m_mutex.unlock();
        return;
    }

    if(!m_pTexture)
    {
        XLOGD("start create  XTexture");
        m_pTexture = XTexture::Create();
        m_pTexture->Init(m_pView, (XTextureType)data.format);
    }
    //XLOGD("start render222222");
    m_pTexture->Draw(data.datas, data.width, data.height);
}


