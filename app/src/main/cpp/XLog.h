//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_XLOG_H
#define XPLAY_XLOG_H
#include <android/log.h>

#define TAG "chx"

class XLog {

};

#ifdef ANDROID

#define XLOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define XLOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define XLOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#else

#endif


#endif //XPLAY_XLOG_H