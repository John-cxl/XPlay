#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>

#include "FFPlayerBuilder.h"

//IVideoView* g_pView = NULL;
IPlayer* g_pPlay = nullptr;
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res)
{
    //FFDecode::InitHard(vm);
    FFPlayerBuilder::InitHard(vm);
    g_pPlay = FFPlayerBuilder::Get()->BuilderPlayer();

    g_pPlay->Open("/sdcard/v1080.mp4");//在这里添加想要播放的文件 路径
    g_pPlay->Start();
    return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_xplay_MainActivity_stringFromJNI(JNIEnv* env,jobject /* this */)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_xplay_XPlay_initView(JNIEnv *env, jobject thiz, jobject view) {
    // TODO: implement initView()
    ANativeWindow* pwin = ANativeWindow_fromSurface(env, view);
    if(g_pPlay) //判断不可靠， 如果是多线程 的话不可靠
        g_pPlay->InitView(pwin);
}