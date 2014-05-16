#include "RenderObject.h"
#include "Renderer.h"


RenderObject::RenderObject(void)	{
	mesh	= NULL;
	shader	= NULL;
	texture = NULL;
	parent  = NULL;
	transparent = false;

	isViewTransformable = true;
}

RenderObject::RenderObject(Mesh*m, Shader*s, GLuint t) {
	mesh	= m;
	shader	= s;
	texture = t;
	parent  = NULL;
	transparent = false;

	isViewTransformable = true;
}


RenderObject::~RenderObject(void)
{
}


void RenderObject::Update(float msec) {

	worldTransform = modelMatrix;

	for(vector<RenderObject*>::const_iterator i = children.begin(); i != children.end(); ++i ) {
		(*i)->Update(msec);
	}
}

bool RenderObject::CollidesWith(RenderObject* obj)
{
	return false;
}

void RenderObject::Draw() const {
	if(mesh) {

		/*if (texture != 0)
			cout << this->GetWorldTransform().GetPositionVector() << endl;*/
		//ActivateLighting();
		mesh->Draw();
	}
}

Matrix4 RenderObject::RetrieveProjectionMatrix()
{
	return Renderer::Instance().GetViewProjectionMatrix(this);
}

void RenderObject::ActivateLighting() const
{
	GLuint program = shader->GetShaderProgram();
	Light light = mesh->GetLight();
	vector<Vector4> colours = light.GetColours();
	vector<Vector3> positions = light.GetPositions();
	vector<float> radii = light.GetRadii();
	glUniform3fv(glGetUniformLocation(program, "lightColour"), colours.size(), (float*)&colours);
	glUniform3fv(glGetUniformLocation(program, "lightPos"), positions.size(), (float*)&positions);
	glUniform1fv(glGetUniformLocation(program, "lightRadius"),radii.size(), (float*)&radii);
}