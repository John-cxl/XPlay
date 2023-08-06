//
// Created by John on 2023-07-18.
//

#ifndef XPLAY_XTEXTURE_H
#define XPLAY_XTEXTURE_H
enum XTextureType
{
    XTEXTURE_YUV420P = 0, //Y 4 U 1 V1
    XTEXTURE_NV12 = 25,   //Y 4 UV1
    XTEXTURE_NV21 = 26    //Y 4 VU1
};

class XTexture {
public:
    static XTexture* Create();
    virtual bool Init(void * pWin, XTextureType type = XTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned  char *data[], int width, int height) = 0;
};


#endif //XPLAY_XTEXTURE_H
