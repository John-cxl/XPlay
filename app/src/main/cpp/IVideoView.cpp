//
// Created by John on 2023-07-18.
//

#include "IVideoView.h"
#include "XLog.h"

void IVideoView::Updata(XData data)
{
//    XLOGD("IVideoView update");
    this->Render(data);
}
