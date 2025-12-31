#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstdlib>
#include <cstdio>


/*
	\brief load a simple 3x3 static texture
	into OpenGL ES.
	\return Returns the handle to the the texture
	object.
*/
GLuint loadTexture();

#endif /* __TEXTURE_H__ */