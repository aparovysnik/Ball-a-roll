#include "TextureResource.h"
#include <sstream>
#include "SOIL.h"

TextureResource::TextureResource()
{
}


TextureResource::~TextureResource()
{
	Unload();
}

TextureResource::TextureResource(const string& addr)
{
	Load(addr);
}

bool TextureResource::Load(const string& addr)
{
	/*int width, height;
	unsigned char* image;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	image = SOIL_load_image(addr.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return true;*/




	bool result;
	result = (surface = IMG_Load(addr.c_str()));
	if (result)
	{
		// Check that the image's width is a power of 2
		if ((surface->w & (surface->w - 1)) != 0) {
			printf("Warning: image's width is not a power of 2.\n");
		}

		// Also check if the height is a power of 2
		if ((surface->h & (surface->h - 1)) != 0) {
			printf("Warning: image's height is not a power of 2.\n");
		}

		// get the number of channels in the SDL surface
		numOfColours = surface->format->BytesPerPixel;
		if (numOfColours == 4)     // contains an alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				textureFormat = GL_RGBA;
			else
				textureFormat = GL_BGRA;
		}
		else if (numOfColours == 3)     // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				textureFormat = GL_RGB;
			else
				textureFormat = GL_BGR;
		}
		else {
			printf("Warning: the image is not truecolor. This will probably break.\n");
		}

		glGenTextures(1, &texture);

		// Bind the texture object
		glBindTexture(GL_TEXTURE_2D, texture);

		// Set the texture's stretching properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0,
			textureFormat, GL_UNSIGNED_BYTE, surface->pixels);


	}
	return(result);
}

GLuint TextureResource::GetTextureID()
{
	return texture;
}

void TextureResource::Unload()
{
	delete surface;
}

SDL_Surface TextureResource::GetTexture()
{
	return *surface;
}

void TextureResource::BindToProgram(const GLuint program, unsigned int GLActiveTexID)
{
	glActiveTexture(GL_TEXTURE0 + GLActiveTexID);
	glBindTexture(GL_TEXTURE_2D, texture);

	std::ostringstream oss;
	oss << "tex" << GLActiveTexID;
	glUniform1i(glGetUniformLocation(program, oss.str().c_str()), GLActiveTexID);
}
