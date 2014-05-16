#include "Texture.h"
#include <sstream>
#include "Mesh.h"
#include "RenderObject.h"
#include "Renderer.h"
#include <list>

//OpenGL
#include "GL/glew.h"
#include "GL/wglew.h"

//SOIL
#include "SOIL.h"

void Texture::BindToProgram(const int texObj, const GLuint program, unsigned int GLActiveTexID)
{
	glActiveTexture(GL_TEXTURE0 + GLActiveTexID);
	glBindTexture(GL_TEXTURE_2D, texObj);


	//glUseProgram(program);
	std::ostringstream oss;
	oss << "tex" << GLActiveTexID;
	GLint test1 = glGetUniformLocation(program, oss.str().c_str());
	glUniform1i(glGetUniformLocation(program, oss.str().c_str()), GLActiveTexID);
	//glUniform1i(glGetUniformLocation(program, "tex2"), 1);

}
