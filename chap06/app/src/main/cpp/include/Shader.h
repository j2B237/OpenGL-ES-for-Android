#ifndef __SHADER_H__
#define __SHADER_H__

#include "GLIncludes.h"
#include "AssetManagerHandler.h"

class Shader{

public:

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void active();
	void deactivate();
	void destroy();
	void loadResource();

	/*[Setter]*/
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setMat4(const char* name, float *matrix);


	/*[Getter]*/
	inline GLuint Id() const {return m_Id;}

private:

	char* loadShaderFromAsset(const char* filename);
	GLuint loadShader(const char *text, GLenum shaderType);
	void createProgram(const char* vertexSource, const char* fragmentSource);

	GLuint m_Id;

	char *m_vertexText, m_fragmentText;
}


#endif /* __SHADER_H__ */
