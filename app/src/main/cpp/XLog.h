//
// Created by John on 2023-07-05.
//

#ifndef XPLAY_XLOG_H
#define XPLAY_XLOG_H
#include <android/log.h>
#include <cstring>

#define TAG "chx"

class XLog {

};

#ifdef ANDROID

#define XLOGD(fmt,...)\
    do{\
        const char* filePath = __FILE__;\
        const char* className = std::strrchr(filePath, '/');\
        className = className ? className + 1 : filePath;\
        __android_log_print(ANDROID_LOG_DEBUG, TAG, "[%s:%d]" fmt,className, __LINE__,##__VA_ARGS__);\
        }while(0)

#define XLOGI(fmt,...) \
    do{ \
        const char* filePath = __FILE__; \
        const char* className = std::strrchr(filePath, '/'); \
        className = className ? className + 1 : filePath; \
        __android_log_print(ANDROID_LOG_INFO, TAG, "%s:%d]" fmt,className, __LINE__,##__VA_ARGS__); \
        }while(0)


#define XLOGE(fmt,...) \
    do{ \
        const char* filePath = __FILE__; \
        const char* className = std::strrchr(filePath, '/'); \
        className = className ? className + 1 : filePath; \
        __android_log_print(ANDROID_LOG_ERROR, TAG, "%s:%d]" fmt,className, __LINE__,##__VA_ARGS__); \
        }while(0)

#else

#endif


#endif //XPLAY_XLOG_H