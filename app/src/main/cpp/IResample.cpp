//
// Created by John on 2023-08-01.
//

#include "IResample.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
    if(d.size >0)
    {
        this->Notify(d);
    }
}
