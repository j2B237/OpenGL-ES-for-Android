#include "include/com_example_lighting_NativeLibrary.h"

#include "include/GLIncludes.h"
#include "include/Matrix.h"


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
