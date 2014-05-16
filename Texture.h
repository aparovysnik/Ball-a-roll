#pragma once
#include <string>
#include "GL/glew.h"
#include "RenderObject.h"




using namespace std;
class Texture
{
public:
	static void BindToProgram(const int texObj, const GLuint program, unsigned int GLActiveTexID);
};
