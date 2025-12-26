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

/*
	createProgram().A program can be thought of as something that holds your shaders. 
	It requires a vertex shader and a fragment shader.

*/

GLuint createProgram(const char *vertexSource, const char *fragmentSource){

	GLuint vertex = loadShader(GL_VERTEX_SHADER, vertexSource);
	if(!vertex)
		return 0;

	GLuint fragment = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
	if(!fragment)
		return 0;

	GLuint program = glCreateProgram();

	if(program){

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		GLint linked = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if(linked != GL_TRUE){
			
			GLint bufLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLen);

			if(bufLen){
				char *buf = (char*)malloc(bufLen * sizeof(char));

				if(buf){
					glGetProgramInfoLog(program, bufLen, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}

/*
	We now define two global variables. The first is the program. 
	This will hold the value of the program that will be created 
	by calling the above functions. 
	The second will hold the location of where the GPU will be expecting 
	the vertex data that is required for our shader.
*/

GLuint simpleTriangleProgram;
GLuint vPosition;

bool setupGraphics(int w, int h) {
	simpleTriangleProgram = createProgram(glVertexShader, glFragmentShader);

	if(!simpleTriangleProgram){
		LOGE("Could not create a program");
		return false;
	}

	vPosition = glGetAttribLocation(simpleTriangleProgram,"vPosition");
	glViewport(0, 0, w, h);

	return true;
}

const GLfloat triangleVertices[] = {
   0.0f, 0.5f,
  -0.5f, -0.5f,
   0.5f, -0.5f
};

void renderFrame() {

	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(simpleTriangleProgram);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, triangleVertices);
	glEnableVertexAttribArray(vPosition);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


JNIEXPORT void JNICALL 
Java_com_example_simpleTriangle_NativeLibrary_init
  (JNIEnv * env, jclass clazz, jint width, jint height) {
	/* sleeping to avoid thrashing the android log */
	setupGraphics(width, height);
}

JNIEXPORT void JNICALL 
Java_com_example_simpleTriangle_NativeLibrary_step
  (JNIEnv *env, jclass clazz){
  	renderFrame();
}
#ifdef __cplusplus
}
#endif
