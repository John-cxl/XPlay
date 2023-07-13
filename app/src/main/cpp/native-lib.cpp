#include <jni.h>
#include <string>
#include <android/log.h>
#include "FFDemux.h"
#include "XLog.h"
#include "FFDecode.h"

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

    TestObserver * ptest = new TestObserver();
    FFDecode* pDecode = new FFDecode();

    FFDemux* pDemux = new FFDemux();
    pDemux->AddObserver(ptest);
    pDemux->open("/sdcard/Enders.Game.2013.BD1080.X264.AAC.English.CHS-ENG.52movieba.mp4");
    pDecode->Open(pDemux->GetVPara());

    pDemux->Start();
    XSleep(3000);
    pDemux->Stop();

    //    for(;;)
//    {
//        XData data = pDemux->read();
//        XLOGI("size = %d ", data.size);
//       data.Drop();
//    }

    //delete(pDemux);
    return env->NewStringUTF(hello.c_str());
}