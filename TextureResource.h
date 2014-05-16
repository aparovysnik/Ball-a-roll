#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include "LOpenGL.h"
#include "Resource.h"

using namespace std;

class TextureResource : public Resource
{
public:
	TextureResource();
	TextureResource(const string& addr);

	~TextureResource();
	
	SDL_Surface GetTexture();
	GLuint GetTextureID();
	bool Load(const string& addr);
	void Unload();

	void BindToProgram(const GLuint program, unsigned int GLActiveTexID);

private:
	SDL_Surface *surface;
	GLuint texture;
	GLenum textureFormat;
	GLint  numOfColours;
};

