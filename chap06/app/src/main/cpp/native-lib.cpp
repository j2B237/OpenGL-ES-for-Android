#include "include/com_example_lighting_NativeLibrary.h"
#include "include/GLIncludes.h"
#include "include/AssetManagerHandler.h"
#include "include/Matrix.h"
#include "include/Shader.h"

#include <android/log.h>

#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


JNIEXPORT void JNICALL Java_com_example_lighting_NativeLibrary_setAssetManager
  (JNIEnv *env, jclass clazz, jclass assetManager){
  
  if(!assetManager){
    LOGE("AssetManager Java is NULL");
    return;
  }

  gAssetManager = AAssetManager_fromJava(env, assetManager);

  if(!gAssetManager){
    LOGE("AAssetManager_fromJava failed!");
    return;
  }
  else{
    LOGI("AAssetManager_fromJava success");
  }
}

JNIEXPORT void JNICALL Java_com_example_lighting_NativeLibrary_initGraphics
  (JNIEnv *env, jclass  clazz, jint width, jint height){

}

JNIEXPORT void JNICALL Java_com_example_lighting_NativeLibrary_loadResources
  (JNIEnv *env, jclass clazz){

}

JNIEXPORT void JNICALL Java_com_example_lighting_NativeLibrary_update
  (JNIEnv *env, jclass clazz, jint width, jint height){

  	glViewport(0, 0, width, height);
  	glEnable(GL_DEPTH_TEST);
}


JNIEXPORT void JNICALL Java_com_example_lighting_NativeLibrary_render
  (JNIEnv *env, jclass clazz){

  	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
