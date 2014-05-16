#pragma once
#include "RenderObject.h"
#include <string>
#include <functional>
#include "LFont.h"

class MenuButton : public RenderObject
{
	friend class RenderObject;
public:
	MenuButton(std::function<void(int)> &fptr, LFont font);

	MenuButton(Mesh *m, Vector3 position, Vector3 scale, Shader *bgShader, Shader *textShader, std::function<void(int)> &fptr, LFont* font, string text, GLuint t = 0);
	~MenuButton();

	void Update(float msec);
	void SetText(string text);
	int GetId();
	void IsEnabled();
	void Enable();
	void Disable();

	void OnButtonClicked();

private:

	std::function<void(int)> onEvent;

	vector<RenderObject*> text;
	LFont font;

	static int current_id;
	int id;

	bool enabled;

};



