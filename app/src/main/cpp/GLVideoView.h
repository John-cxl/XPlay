//
// Created by John on 2023-07-18.
//

#ifndef XPLAY_GLVIDEOVIEW_H
#define XPLAY_GLVIDEOVIEW_H


#include "XData.h"
#include "IVideoView.h"

class XTexture;
class GLVideoView :public IVideoView{
public:
    virtual void SetRender(void * pWin);
    virtual void Render(XData data);

protected:
    void * m_pView = 0;
    XTexture * m_pTexture;
};


#endif //XPLAY_GLVIDEOVIEW_H
