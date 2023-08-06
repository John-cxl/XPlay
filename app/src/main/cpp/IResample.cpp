//
// Created by John on 2023-08-01.
//

#include "IResample.h"
#include "XLog.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
    //XLOGD("iresample size = %d, d size = %d",data.size, d.size );
    if(d.size > 0)
    {
        this->Notify(d);
    }
}
