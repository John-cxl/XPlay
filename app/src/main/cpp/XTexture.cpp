//
// Created by John on 2023-07-18.
//

#include "XTexture.h"
#include "XEGL.h"
#include "XLog.h"
#include "XShader.h"

class CXTextrue :public XTexture
{
public:
    XShader sh;
    virtual bool Init(void * pWin)
    {
        if(!pWin)
        {
            XLOGE("CXTextrue INit failed\n");
            return false;
        }

        if(!XEGL::Get()->Init(pWin))
        {
            XLOGE("XEGL INit failed\n");
            return false;
        }

        if(!sh.Init())
        {
            XLOGE("shader INit failed\n");
            return false;
        }

        XLOGE("shader INit success )))))))))))))))))))))\n");
        return true;
    }
    virtual void Draw(unsigned  char *data[], int width, int height)
    {
        sh.GetTexture(0, width, height,data[0]); //y
        sh.GetTexture(1, width/2, height/2,data[1]); //u
        sh.GetTexture(2, width/2, height/2,data[2]); //v
        sh.Draw();
        //XLOGE("start Draw");
        XEGL::Get()->Draw();
    }
};

XTexture *XTexture::Create() {
    return new CXTextrue();
}
