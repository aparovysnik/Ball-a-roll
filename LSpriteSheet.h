/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 002

#ifndef LSPRITE_SHEET_H
#define LSPRITE_SHEET_H

#include "LTexture.h"
#include "LVertexData2D.h"
#include <vector>
#include "RenderObject.h"

//Sprite drawing origin
enum LSpriteOrigin
{
    LSPRITE_ORIGIN_CENTER,
    LSPRITE_ORIGIN_TOP_LEFT,
    LSPRITE_ORIGIN_BOTTOM_LEFT,
    LSPRITE_ORIGIN_TOP_RIGHT,
    LSPRITE_ORIGIN_BOTTOM_RIGHT
};

class LSpriteSheet : public LTexture
{
	public:
		LSpriteSheet();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Initializes buffer ID
        Side Effects:
         -None
        */

		~LSpriteSheet();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Deallocates sprite sheet data
        Side Effects:
         -None
        */

		int addClipSprite( LFRect& newClip );
        /*
        Pre Condition:
         -None
        Post Condition:
         -Adds clipping rectangle to clip array
         -Returns index of clipping rectangle within clip array
        Side Effects:
         -None
        */
		Mesh* getCharacter(LSpriteOrigin origin, GLuint charCode, Vector4 col[4]);
		LFRect getClip( int index );
        /*
        Pre Condition:
         -A valid index
        Post Condition:
         -Returns clipping clipping rectangle at given index
        Side Effects:
         -None
        */

		bool generateDataBuffer( LSpriteOrigin origin = LSPRITE_ORIGIN_CENTER );
        /*
        Pre Condition:
         -A loaded base LTexture
         -Clipping rectangles in clip array
        Post Condition:
         -Generates VBO and IBO to render sprites with
         -Sets given origin for each sprite
         -Returns true on success
         -Reports to console is an error occured
        Side Effects:
         -Member buffers are bound
        */

		void freeSheet();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Deallocates member VBO, IBO, and clip array
        Side Effects:
         -None
        */

        void freeTexture();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Frees sprite sheet and base LTexture
        Side Effects:
         -None
        */

		void renderSprite( int index );
        /*
        Pre Condition:
         -Loaded base LTexture
         -Generated VBO
        Post Condition:
         -Renders sprite at given index
        Side Effects:
         -Base LTexture is bound
         -Member buffers are bound
        */

		//Sprite clips
		std::vector<LFRect> mClips;

		//VBO data
		GLuint mVertexDataBuffer;
		GLuint* mIndexBuffers;

		Mesh** quadsPtr;


		bool generateTextDisplay(LSpriteOrigin origin, Vector4 col[4]);

	protected:
		int mShrinkRate;
};

#endif
