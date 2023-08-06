//
// Created by John on 2023-07-18.
//

#ifndef XPLAY_XSHADER_H
#define XPLAY_XSHADER_H
enum XShaderType
{
    XSHADER_YUV420P = 0,
    XSHADER_NV12    = 25,
    XSHADER_NV21    = 26
};

class XShader {
public:
    virtual bool Init(XShaderType type = XSHADER_YUV420P);
    //获取材质并映射到内存
    virtual void GetTexture(unsigned int index, int width , int height, unsigned char* buf, bool isa=false);
    virtual void Draw();
private:
    unsigned int m_vsh = 0;   //顶点着色器
    unsigned int m_fsh = 0;   //片源着色器
    unsigned int m_program = 0;
    unsigned int texts[100] =  {0};
};


#endif //XPLAY_XSHADER_H
