#include "com_example_graphicsSetup_NativeLibrary.h"
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL 
Java_com_example_graphicsSetup_NativeLibrary_init
  (JNIEnv * env, jclass clazz, jint width, jint height)
{
	/* sleeping to avoid thrashing the android log */
	sleep(5),
	LOGI("New Frame ready to be Drawn!!!!");
}

JNIEXPORT void JNICALL 
Java_com_example_graphicsSetup_NativeLibrary_step
  (JNIEnv *env, jclass clazz)
{

}
#ifdef __cplusplus
}
#endif
