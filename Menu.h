#pragma once
#include "RenderObject.h"
#include "MenuButton.h"
#include "ResourceManager.h"
#include "LFont.h"
#include <map>


class Menu : public RenderObject
{
public:
	Menu(Shader* s, LFont* font, int renderAreaWidth, int renderAreaHeight, GLuint textureId);
	~Menu();

	void Update(float msec);
	void AddButton(Mesh *m, Vector3 position, Vector3 scale, Shader *bgShader, Shader *textShader, std::function<void(int)> &fptr, LFont* font, string text, GLuint t = 0);
	void AddButton(MenuButton* btn);

	MenuButton* GetButton(int id)	{ return mButtons[id]; }
private:
	map<int, MenuButton*> mButtons;
	LFont* font;
	int width;
	int height;

	RenderObject* cursor;
};

