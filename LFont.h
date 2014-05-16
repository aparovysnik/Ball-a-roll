/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 003

#ifndef LFONT_H
#define LFONT_H

#include "LSpriteSheet.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Mesh.h"
#include "Renderer.h"

class LFont : private LSpriteSheet
{
	public:
        static bool initFreeType();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Instantiates FreeType library used for every LFont object
        Side Effects:
         -None
        */

		LFont();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Initializes spacing variables
        Side Effects:
         -None
        */

		~LFont();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Frees font
        Side Effects:
         -None
        */

		bool loadBitmap( std::string path );
        /*
        Pre Condition:
         -None
        Post Condition:
         -Loads bitmap font
         -Returns true on success
         -Reports errors to console
        Side Effects:
         -None
        */

        bool loadFreeType( std::string path, GLuint pixelSize, int shrinkRate, Vector4 col[4]);
        /*
        Pre Condition:
         -None
        Post Condition:
         -Creates internal 8bit texture with given font file
         -Reports freetype error code if freetype error occurs
        Side Effects:
         -None
        */

        void freeFont();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Deallocates texture and sprite sheet data
        Side Effects:
         -None
        */

        void renderText( GLfloat x, GLfloat y, GLfloat z, std::string text, Renderer* r, Shader* s);
        /*
        Pre Condition:
         -A loaded font
        Post Condition:
         -Renders text
        Side Effects:
         -Binds member texture and data buffers
        */

		std::vector<RenderObject*> renderText(GLfloat x, GLfloat y, GLfloat z, std::string text, Shader* s);

		GLfloat GetLineHeight();

		std::vector<float> GetTextDimensions(std::string text);

	private:
        //Font TTF library
        static FT_Library mLibrary;

        //Spacing variables
        GLfloat mSpace;
        GLfloat mLineHeight;
        GLfloat mNewLine;
		Vector4 colour[4];
};

#endif
