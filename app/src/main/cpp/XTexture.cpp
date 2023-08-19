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
    XTextureType type;
    std::mutex m_mutex;

    virtual void Drop()
    {
        m_mutex.lock();
        XEGL::Get()->Close();
        sh.Close();
        m_mutex.unlock();
        delete this;
    }
    virtual bool Init(void * pWin, XTextureType type)
    {
        m_mutex.lock();
        XEGL::Get()->Close();
        sh.Close();
        this->type = type;
        if(!pWin)
        {
            m_mutex.unlock();
            XLOGE("CXTextrue INit failed\n");
            return false;
        }

        if(!XEGL::Get()->Init(pWin))
        {
            m_mutex.unlock();
            XLOGE("XEGL INit failed\n");
            return false;
        }

//        if(!sh.Init())
//        {
//            XLOGE("shader INit failed\n");
//            return false;
//        }
        sh.Init((XShaderType)type);
        m_mutex.unlock();
        XLOGE("shader INit success )))))))))))))))))))))\n");

        return true;
    }
    virtual void Draw(unsigned  char *data[], int width, int height)
    {
        m_mutex.lock();
        sh.GetTexture(0, width, height,data[0]); //y
        if(type == XTEXTURE_YUV420P)
        {
            sh.GetTexture(1, width/2, height/2,data[1]); //u
            sh.GetTexture(2, width/2, height/2,data[2]); //v
        }else
        {
            sh.GetTexture(1, width/2, height/2,data[1], true); //uv
        }


        sh.Draw();
        //XLOGE("start Draw");
        XEGL::Get()->Draw();
        m_mutex.unlock();
    }
};

XTexture *XTexture::Create() {
    return new CXTextrue();
}
