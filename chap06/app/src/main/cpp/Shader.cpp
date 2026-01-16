#include "include/Shader.h"

#define LOG_TAG "SHADER"

#define LOGE(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

Shader::Shader(const char *vertexPath, const char *fragmentPath) :
	m_Id(0)
{
	m_vertexText = loadShaderFromAsset(vertexPath);
	m_fragmentText = loadShaderFromAsset(fragmentPath);

	if(!m_vertexText || !m_fragmentText){
		LOGE("Could not vertex or fragment from asset");
		return;
	}

	createProgram(m_vertexText, m_fragmentText);
}

Shader::~Shader(){

	LOGE("Instance destroyed");
	destroy();
}

void Shader::active(){

	if (m_Id != 0){
		glUseProgram(m_Id);
	}
}

void Shader::deactivate(){

	if(m_Id != 0)
		glUseProgram(0);
}

void Shader::destroy(){

	if(m_vertexText)
		m_vertexText = nullptr;
	if(m_fragmentText)
		m_fragmentText = nullptr;
	if(m_Id)
		m_Id = 0;
}

void Shader::loadResource(){
	createProgram(m_vertexText, m_fragmentText);
}


char* Shader::loadShaderFromAsset(const char *filename){

	if(!gAssetManager){
		LOGE("AAssetManager_fromJava failed");
		return nullptr;
	}

	AAsset *asset = AAssetManager_open(
		gAssetManager, filename, AASSET_MODE_BUFFER)
	;

	if(!asset){
		LOGE("Impossible to open %s", filename);
		return nullptr;
	}

	off_t lenght = AAsset_getLength(asset);
	char *buffer = (char*)malloc(lenght+1);

	if(!buffer){
		LOGE("malloc() failed!");
		return nullptr;
	}

	AAsset_read(asset, buffer, lenght);
	buffer[lenght] = '\0';

	AAsset_close(asset);
	return buffer;
}

void Shader::createProgram(const char* vertexSource, const char* fragmentSource){

	GLuint vertex = loadShader(vertexSource, GL_VERTEX_SHADER);
	if(vertex)
		return;

	GLuint fragment = loadShader(fragmentSource, GL_FRAGMENT_SHADER);
	if(fragment)
		return;

	m_Id = glCreateProgram();

	if(m_Id){

		glAttachShader(m_Id, vertex);
		glAttachShader(m_Id, fragment);

		glLinkProgram(m_Id);

		GLint linked = GL_FALSE;
		glGetProgramiv(m_Id, GL_LINK_STATUS, &linked);

		if(linked != GL_TRUE){

			GLint lenght = 0;
			glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &lenght);

			if(lenght > 0){
				char *buffer = (char*)malloc(lenght);

				if(buffer){

					glGetProgramInfoLog(m_Id, lenght, NULL, buffer);
					LOGE("ERROR::PROGRAM:\n%s", buffer);
					free(buffer);
				}
				glDeleteProgram(m_Id);
				m_Id = 0;
			}	
		}
	}
}


GLuint Shader::loadShader(const char *text, GLenum shaderType){

	GLuint shader = glCreateShader(shaderType);	

	if(shader){
		const char *type = (shaderType == GL_FRAGMENT_SHADER) ? 
							"FRAGMENT" :  "VERTEX";

		glShaderSource(shader, 1, &text, NULL);
		glCompileShader(shader);
		checkShaderError(shader, type);
	}
	return shader;
}

void Shader::checkShaderError(GLuint shader, const char* type){

	if(strcmp(type,"PROGRAM") == 0) {

		GLint compiled = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled){

			GLint lenght = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);

			if(lenght > 0){
				char *buffer = (char*)malloc(lenght);

				if(buffer){
					glGetShaderInfoLog(shader, lenght, NULL, buffer);
					LOGE("ERROR::SHADER:\n%s", buffer);
					free(buffer);
				}
				glDeleteShader(shader);
				shader = 0;
			}	
		}
	}
}


void Shader::setInt(const char* name, int value){
	GLint location = glGetUniformLocation(m_Id, name);
	glUniform1i(location, value);
}

void Shader::setFloat(const char* name, float value){
	GLint location = glGetUniformLocation(m_Id, name);
	glUniform1f(location, value);
}

void Shader::setMat4(const char* name, float *matrix){
	GLint location = glGetUniformLocation(m_Id, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}