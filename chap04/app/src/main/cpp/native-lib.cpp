#include "com_example_simpleCube_NativeLibrary.h"
#include "include/matrix.h"

#include <unistd.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#define LOG_TAG "libNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


/* Hardcoded vertex and fragment shader programs */

/* 
	This is the source code for your vertex shader. 
	The first line declares vPosition as an attribute. 
	This is an input to the shader and you will have to provide 
	the data to the shader from your Java or C++ program
*/

static const char glVertexShader[] = 
	"attribute vec4 vertexPosition;\n"
	"attribute vec3 vertexColour;\n"
	"varying vec3 fragColour;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 modelView;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = projection * modelView * vertexPosition;\n"
	"    fragColour = vertexColour;\n"
	"}\n";

/*
	This is the source code for your fragment shader. 
	The first line is required because you need to set the precision of your shader. 
	This can be either lowp, mediump or highp.
*/

static const char glFragmentShader[] = 
	"precision mediump float;\n"
	"varying vec3 fragColour;\n"
	"void main()\n"
	"{\n"
	"    gl_FragColor = vec4(fragColour, 1.0);\n"
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

GLuint simpleCubeProgram;
GLuint vertexLocation, vertexColorLocation;
GLuint projectionLocation, modelViewLocation;

float projectionMatrix[16];
float modelViewMatrix[16];
float angle = 0;

bool setupGraphics(int w, int h) {

	simpleCubeProgram = createProgram(glVertexShader, glFragmentShader);

	if(!simpleCubeProgram){
		LOGE("Could not create a program");
		return false;
	}

	vertexLocation = glGetAttribLocation(simpleCubeProgram,"vertexPosition");
	vertexColorLocation = glGetAttribLocation(simpleCubeProgram, "vertexColour");
	projectionLocation = glGetUniformLocation(simpleCubeProgram, "projection");
	modelViewLocation = glGetUniformLocation(simpleCubeProgram, "modelView");

	// setup the perspective
	matrixPerspective(projectionMatrix, 45.0f, (float)w/(float)h, 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, w, h);

	return true;
}

GLfloat cubeVertices[] = {
	-0.5f,  0.5f, -0.5f, /* Back. */
   0.5f,  0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f, /* Front. */
	 0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f, /* Left. */
  -0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f, /* Right. */
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
 	 0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f, /* Top. */
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
 	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f, /* Bottom. */
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f
};

GLfloat colour[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f
 };


GLushort indices[] = {
	0, 2, 3, 
	0, 1, 3, 
	4, 6, 7, 
	4, 5, 7,
	8, 9, 10, 
	11, 8, 10, 
	12, 13, 14, 
	15, 12, 14, 
	16, 17, 18, 
	16, 19, 18, 
	20, 21, 22, 
	20, 23, 22
};


void renderFrame() {

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	matrixIdentityFunction(modelViewMatrix);

	matrixRotateX(modelViewMatrix, angle);
	matrixRotateY(modelViewMatrix, angle);

	matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f);
	//matrixScale(modelViewMatrix, 1.5f, 1.5f, 1.5f);

	glUseProgram(simpleCubeProgram);
	
	// Bind and enable both vertexPosition and vertexColor
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);
	glEnableVertexAttribArray(vertexLocation);

	glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 0, colour);
	glEnableVertexAttribArray(vertexColorLocation);


	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);

	angle += 1.0;
	if(angle > 360.0)
		angle -= 360.0;
}


JNIEXPORT void JNICALL 
Java_com_example_simpleCube_NativeLibrary_init
  (JNIEnv * env, jclass clazz, jint width, jint height) {
	setupGraphics(width, height);
}

JNIEXPORT void JNICALL 
Java_com_example_simpleCube_NativeLibrary_step
  (JNIEnv *env, jclass clazz){
  	renderFrame();
}

