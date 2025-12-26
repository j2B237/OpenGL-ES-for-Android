#include "com_example_simpleTriangle_NativeLibrary.h"
#include <unistd.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif


/* Hardcoded vertex and fragment shader programs */

/* 
	This is the source code for your vertex shader. 
	The first line declares vPosition as an attribute. 
	This is an input to the shader and you will have to provide 
	the data to the shader from your Java or C++ program
*/

static const char glVertexShader[] = 
	"attribute vec4 vPosition;\n"
	"void main(){\n"
	"\tgl_Position = vPosition;\n"
	"}\n";

/*
	This is the source code for your fragment shader. 
	The first line is required because you need to set the precision of your shader. 
	This can be either lowp, mediump or highp.
*/

static const char glFragmentShader[] = 
	"precision mediump float;\n"
	"void main(){\n"
	"\tgl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
	"}\n";


/* 
	loadShader create a shader type, loads it's source code from a file
	compile them into a form that can run on GPU. Finally return an OpenGL ID 
	of the compile shader object.
*/

GLuint loadShader(GLenum shaderType, const char *shaderSource){

	GLuint shader = glCreateShader(shaderType);
	
	if(shader){
		
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled){
			
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if(infoLen){
				char *buf = (char*)malloc(infoLen * sizeof(char));

				if(buf){
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}

	return shader;
}


JNIEXPORT void JNICALL 
Java_com_example_simpleTriangle_NativeLibrary_init
  (JNIEnv * env, jclass clazz, jint width, jint height)
{
	/* sleeping to avoid thrashing the android log */
	sleep(5),
	LOGI("New Frame ready to be Drawn!!!!");
}

JNIEXPORT void JNICALL 
Java_com_example_simpleTriangle_NativeLibrary_step
  (JNIEnv *env, jclass clazz)
{

}
#ifdef __cplusplus
}
#endif
