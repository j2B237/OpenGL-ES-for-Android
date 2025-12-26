#include "com_example_chap01_NativeLibrary.h"


#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL 
Java_com_example_chap01_NativeLibrary_init
  (JNIEnv *, jclass clazz)
{
	LOGI("Hello From Native side!");
}

#ifdef __cplusplus
}
#endif
