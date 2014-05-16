#include "MenuButton.h"
#include "Application.h"

int MenuButton::current_id = 0;

MenuButton::MenuButton(std::function<void(int)> &fptr, LFont font)
{
	onEvent = fptr;
	this->font = font;
	ToggleViewTransformation(false);
}

MenuButton::MenuButton(Mesh *m, Vector3 position, Vector3 scale, Shader *bgShader, Shader *textShader, std::function<void(int)> &fptr, LFont* font, string text, GLuint t) : RenderObject(m, bgShader, t)
{
	//Set things to do when clicked
	onEvent = fptr;
	modelMatrix.SetPositionVector(position);
	modelMatrix.SetScalingVector(scale);
	//Generate button ID
	id = current_id++;

	//Calculate text position
	Vector3 pos = position;
	Vector3 sc = scale;

	vector<float> textDim = font->GetTextDimensions(text);

	int textX = pos.x - (textDim.at(0) * 0.5);
	int textY = pos.y - (textDim.at(1) * 0.5);

	//Set text
	this->text = font->renderText(textX, textY, pos.z, text, textShader);
	RenderObject* container = new RenderObject();
	container->ToggleViewTransformation(false);
	for each(RenderObject* obj in this->text)
	{
		container->AddChild(obj);
		obj->ToggleViewTransformation(false);
	}
	this->AddChild(container);
	ToggleViewTransformation(false);
}

MenuButton::~MenuButton()
{
}

int MenuButton::GetId()
{
	return id;
}

void MenuButton::Update(float msec)
{
	Mouse* mouse = Window::GetMouse();
	worldTransform = modelMatrix;
	for (vector<RenderObject*>::const_iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}

	if (mouse->ButtonDown(MouseButtons::MOUSE_LEFT) && !mouse->ButtonHeld(MouseButtons::MOUSE_LEFT))
	{
		BoundingBox b = mesh->CalculateBoundingBox(worldTransform);
		Vector2 mPos = mouse->GetAbsolutePosition();
		//Invert y coordinate
		mPos.y = parent->GetModelMatrix().GetScalingVector().y - mPos.y;
		
		//Check if the click was performed inside the area of the button.
		if ((mPos.x >= b.backBottomLeft.x) && (mPos.x <= b.frontTopRight.x) &&
			(mPos.y >= b.backBottomLeft.y) && (mPos.y <= b.frontTopRight.y))
		{
			OnButtonClicked(); //Raise event
		}
	}
}

void MenuButton::OnButtonClicked()
{
	onEvent(id);
}
