#include <jni.h>
#include <string>
#include <android/log.h>
#include "FFDemux.h"
#include "XLog.h"
#include "FFDecode.h"
#include <android/native_window_jni.h>

class TestObserver :public IObserver
{
    void Updata(XData data) override
    {
        //XLOGI("testObserver data size = %d", data.size);
    }
};



extern "C" JNIEXPORT jstring JNICALL
Java_com_example_xplay_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    IDemux* pDemux = new FFDemux();
    pDemux->open("/sdcard/Enders.Game.2013.BD1080.X264.AAC.English.CHS-ENG.52movieba.mp4");

    IDecode* pVDecode = new FFDecode();
    pVDecode->Open(pDemux->GetVPara()); //获取

    IDecode* pADecode = new FFDecode();
    pADecode->Open( pDemux->GetAPara());

    pDemux->AddObserver(pVDecode);  //添加观察者
    pDemux->AddObserver(pADecode);  //添加观察者

    pDemux->Start();
    pVDecode->Start();
    pADecode->Start();


//    pDemux->Start();
//    XSleep(3000);
//    pDemux->Stop();

    //    for(;;)
//    {
//        XData data = pDemux->read();
//        XLOGI("size = %d ", data.size);
//       data.Drop();
//    }

    //delete(pDemux);
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_xplay_XPlay_initView(JNIEnv *env, jobject thiz, jobject view) {
    // TODO: implement initView()

    ANativeWindow* pwin = ANativeWindow_fromSurface(env, view);



}