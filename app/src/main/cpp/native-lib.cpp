#include <jni.h>
#include <string>
#include <android/log.h>
#include "FFDemux.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_xplay_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    FFDemux* pDemux = new FFDemux();
    pDemux->open("/sdcard/Enders.Game.2013.BD1080.X264.AAC.English.CHS-ENG.52movieba.mp4");
    delete(pDemux);
    return env->NewStringUTF(hello.c_str());
}