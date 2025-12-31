#include "com_example_texturedcube_NativeLibrary.h"
#include "include/Matrix.h"
#include "include/Texture.h"

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
	"attribute vec2 vertexTexCoord;\n"
	"varying vec2 textureCoord;\n"
	"uniform mat4 projection;\n"
	"uniform mat4 modelView;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = projection * modelView * vertexPosition;\n"
	"    textureCoord = vertexTexCoord;\n"
	"}\n";

/*
	This is the source code for your fragment shader. 
	The first line is required because you need to set the precision of your shader. 
	This can be either lowp, mediump or highp.
*/

static const char glFragmentShader[] = 
	"precision mediump float;\n"
	"uniform sampler2D texture;\n"
	"varying vec2 textureCoord;\n"
	"void main()\n"
	"{\n"
	"    gl_FragColor = texture2D(texture, textureCoord);\n"
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

// Redering program object handle
GLuint simpleProgram;

// Pointer to object in GPU memory
GLuint vertexLocation, vertexTexCoordLoc;
GLuint projectionLocation, modelViewLocation;

// texture object handle
GLuint samplerLocation;
GLuint textureId;															


float projectionMatrix[16];
float modelViewMatrix[16];
float angle = 0;

bool setupGraphics(int w, int h) {

	simpleProgram = createProgram(glVertexShader, glFragmentShader);

	if(!simpleProgram){
		LOGE("Could not create a program");
		return false;
	}

	// Retrieve Address of objects in GPU memory
	vertexLocation = glGetAttribLocation(simpleProgram,"vertexPosition");
	vertexTexCoordLoc = glGetAttribLocation(simpleProgram, "vertexTexCoord");
	projectionLocation = glGetUniformLocation(simpleProgram, "projection");
	modelViewLocation = glGetUniformLocation(simpleProgram, "modelView");
	samplerLocation = glGetUniformLocation(simpleProgram, "texture");

	// setup the perspective
	matrixPerspective(projectionMatrix, 45.0f, (float)w/(float)h, 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, w, h);

	// Load the texture
	textureId = loadTexture();
	if(textureId == 0)
		return false;
	else
		return true;

}

GLfloat cubeVertices[108] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};


GLfloat textureCords[] = { 
	1.0f, 1.0f, /* Back. */
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f, /* Front. */
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f, /* Left. */
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f, /* Right. */
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f, /* Top. */
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f, /* Bottom. */
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};


void renderFrame() {

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	matrixIdentityFunction(modelViewMatrix);

	matrixRotateX(modelViewMatrix, angle);
	matrixRotateY(modelViewMatrix, angle);

	matrixTranslate(modelViewMatrix, 0.0f, 0.0f, -10.0f);
	//matrixScale(modelViewMatrix, 1.5f, 1.5f, 1.5f);

	glUseProgram(simpleProgram);
	
	// Bind and enable both vertexPosition and vertexColor
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, cubeVertices);
	glEnableVertexAttribArray(vertexLocation);

	glVertexAttribPointer(vertexTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, textureCords);
	glEnableVertexAttribArray(vertexTexCoordLoc);

	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelViewMatrix);

	// Set the sampler texture unit to 0 */
	glUniform1i(samplerLocation, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);

	angle += 1.0;
	if(angle > 360.0)
		angle -= 360.0;
}


JNIEXPORT void JNICALL 
Java_com_example_texturedcube_NativeLibrary_init
  (JNIEnv * env, jclass clazz, jint width, jint height) {
	setupGraphics(width, height);
}

JNIEXPORT void JNICALL 
Java_com_example_texturedcube_NativeLibrary_step
  (JNIEnv *env, jclass clazz){
  	renderFrame();
}

