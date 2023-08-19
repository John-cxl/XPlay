#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>

#include "IPlayerPorxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res)
{
    IPlayerPorxy::Get()->Init(vm);
    //IPlayerPorxy::Get()->Open("/sdcard/Enders.Game.2013.BD1080.X264.AAC.English.CHS-ENG.52movieba.mp4");
    //IPlayerPorxy::Get()->Open("/sdcard/v1080.mp4");//在这里添加想要播放的文件 路径
    //IPlayerPorxy::Get()->Open("rtmp://58.200.131.2:1935/livetv/cctv6hd");
    //IPlayerPorxy::Get()->Start();
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
    IPlayerPorxy::Get()->InitView(pwin);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_xplay_open_1source_Open(JNIEnv *env, jobject thiz, jstring url)
{
    // TODO: implement Open()
    const char* cUrl = env->GetStringUTFChars(url,0);

    IPlayerPorxy::Get()->Open(cUrl);
    IPlayerPorxy::Get()->Start();
    //IPlayerPorxy::Get()->Seek(0.5);

    env->ReleaseStringUTFChars(url, cUrl);
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_xplay_MainActivity_PlayPos(JNIEnv *env, jobject thiz) {
    // TODO: implement PlayPos()

    return IPlayerPorxy::Get()->PlayPos();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_xplay_MainActivity_Seek(JNIEnv *env, jobject thiz, jdouble pos) {
    // TODO: implement Seek()
    IPlayerPorxy::Get()->Seek(pos);
}