#include "Menu.h"


Menu::Menu(Shader* s, LFont* font, int renderAreaWidth, int renderAreaHeight, GLuint textureId)
{
	this->shader = s;
	this->width = renderAreaWidth;
	this->height = renderAreaHeight;
	this->font = font;
	this->texture = textureId;
	this->mesh = Mesh::GenerateQuad();
	this->modelMatrix = (Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Scale(Vector3(width, height, 1)));

	TextureResource* t = ResourceManager::Instance().GetResource<TextureResource>("triforcecursor.png");
	this->cursor = new RenderObject(Mesh::GenerateQuad(), s, t->GetTextureID());
	cursor->SetModelMatrix(Matrix4::Scale(Vector3(10, 10, 0)));
	cursor->ToggleViewTransformation(false);
	AddChild(cursor);
	ToggleViewTransformation(false);
}


Menu::~Menu()
{
	for (std::map<int, MenuButton*>::iterator it = mButtons.begin(); it != mButtons.end(); ++it)
	{
		delete it->second;
	}
}

void Menu::Update(float msec)
{
	Mouse* mouse = Window::GetMouse();
	Vector2 cPos = mouse->GetAbsolutePosition();
	cursor->SetPosition(Vector3(cPos.x, GetModelMatrix().GetScalingVector().y - cPos.y, 0));
		
	worldTransform = modelMatrix;

	for (vector<RenderObject*>::const_iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void Menu::AddButton(Mesh *m, Vector3 position, Vector3 scale, Shader *bgShader, Shader *textShader, std::function<void(int)> &fptr, LFont* font, string text, GLuint t)
{
	MenuButton* button = new MenuButton(m, position, scale, bgShader, textShader, fptr, font, text, t);
	AddChild(button, children.size() - 1);
	mButtons.insert(std::pair<int, MenuButton*>(button->GetId(), button));
}

void Menu::AddButton(MenuButton* btn)
{
	AddChild(btn, children.size() - 1);
	mButtons[btn->GetId()] = btn;
}
