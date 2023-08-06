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

void GLVideoView::Render(XData data)
{
//    XLOGD("start render1111111");
    if(!m_pView) return;
    if(!m_pTexture)
    {
        XLOGD("start create  XTexture");
        m_pTexture = XTexture::Create();
        m_pTexture->Init(m_pView, (XTextureType)data.format);
    }
    //XLOGD("start render222222");
    m_pTexture->Draw(data.datas, data.width, data.height);

}
