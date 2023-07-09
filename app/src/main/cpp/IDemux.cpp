//
// Created by John on 2023-07-05.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main()
{
    while(!isExit)
    {
        XData data = read();
//        XLOGI("size = %d", data.size);
//        if(0 >= data.size){break;}
    }
}