#include "Renderer.h"
#include "Texture.h"
#include "PhysicsContainer.h"
#include "Application.h"

Renderer::Renderer(Window &parent, Environment* env) : CAM_MOVEMENT_RATE(0.4), INV_CAM_MOVEMENT_RATE(-0.4), ROTATION_RATE(1), INV_ROTATION_RATE(-1), OGLRenderer(parent)	{

	Vector2 windowSize = parent.GetScreenSize();
	this->orthoProj = Matrix4::Orthographic(-1, 1, windowSize.x, 0, windowSize.y, 0);
	this->orthoView = Matrix4::Translation(Vector3(1, 0, -1));

	//modelMatrix = 	//Matrix4::Rotation(90,Vector3(1,0,0)) * 
	//	Matrix4::Scale(Vector3(100,100,100));
	this->environment = env;
	camRotation = 0;
	viewMatrix = Matrix4::Rotation(70.0f, Vector3(1,0,0)) * 
		Matrix4::Rotation(-225.0f, Vector3(0,1,0)) * 
		Matrix4::Translation(Vector3(100,-400,100));

	projMatrix = Matrix4::Perspective(1.0f,1000.0f,
		(float)width/(float)height,45.0f);

	//Shader* s	= new Shader("tessVert.glsl", "displaceFrag.glsl","", "displaceTCS.glsl", "displaceTES.glsl");
	//Shader* s	= new Shader("tessVert.glsl", "displaceFrag.glsl");

	/*GLuint tex 	= LoadTexture("perlinNoise.png");

	RenderObject *o = new RenderObject(m,s, tex);
	o->SetModelMatrix(Matrix4::Rotation(90,Vector3(1,0,0)) * Matrix4::Scale(Vector3(100,100,100)));*/

	//AddRenderObject(*o);
}

Renderer::~Renderer(void)	{

}

GLuint Renderer::LoadTexture(string filename) {
	return 	SOIL_load_OGL_texture(filename.c_str(),SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS);
}

void	Renderer::RenderScene() {

	//Render opaque entities
	vector<GameEntity*> opaqueEntities = environment->GetOpaqueEntities();
	int i = 0;
	while (i < opaqueEntities.size())
	{
		GameEntity* entity = opaqueEntities[i];
		if (!entity->IsOneFrameOnly())
			i++;
		else
			opaqueEntities.erase(opaqueEntities.begin() + i);
		RenderEntity(entity);
	}

	//Now render transparent entities
	vector<GameEntity*> transparentEntities = environment->GetTransparentEntities();
	i = 0;
	while (i < transparentEntities.size())
	{
		GameEntity* entity = transparentEntities[i];
		if (!entity->IsOneFrameOnly())
			i++;
		else
			transparentEntities.erase(transparentEntities.begin() + i);
		RenderEntity(entity);
	}
	/*for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		Render(*(*i));
	}*/
}

void Renderer::RenderEntity(GameEntity *entity)
{
	if (entity->IsVisible())
	{
		if (entity->HasVisuals())
			Render(*entity->GetVisuals());
		vector<GameEntity*> children = entity->GetChildren();
		for each(GameEntity *child in children)
			RenderEntity(child);
	}
	if (entity->IsOneFrameOnly())
		environment->RemoveEntity(entity);
}

void	Renderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glFrontFace(GL_CW);

	if(o.GetShader() && o.GetMesh()) {
		GLuint program = o.GetShader()->GetShaderProgram();
		glUseProgram(program);

		Texture::BindToProgram(o.GetTexture(), program, 0);

		if (!o.IsTransformable())
		{
			Matrix4 backupP = projMatrix;
			Matrix4 backupV = viewMatrix;
			Vector2 windowSize = Application::Instance().GetScreenSize();
			SetProjectionMatrix(orthoProj);
			SetViewMatrix(orthoView);

			UpdateShaderMatrices(program);
			projMatrix = backupP;
			viewMatrix = backupV;
		}
		else
		{
			UpdateShaderMatrices(program);
		}

		//glActiveTexture(GL_TEXTURE0);
		//GLuint tex = o.GetTexture();
		//glBindTexture(GL_TEXTURE_2D, tex);
		//GLint texLoc = glGetUniformLocation(program, "tex0");
		//glUniform1i(texLoc, 0);
		o.Draw();
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) {
	PhysicsContainer* p = &PhysicsContainer::Instance();

	//Update object visuals

	environment->begin();
	GameEntity *entity = environment->GetNextEntity();

	while (entity != NULL)
	{
		entity->UpdateVisuals(msec);
		entity = environment->GetNextEntity();
	}

	//Get camera actions
	set<Action> cameraActions = Controller::Instance().GetGlobalActions();

	//Move with camera object if any

	//Rotate around camera object if any
	Vector3 camOrigin = Vector3(0, 0, 0);

	if (p->ContainsCamObject())
	{
		Vector3 camMove = p->GetCamObjectVelocity();
		camOrigin = p->GetCamObjectOrigin();
		//Rotate to the direction the camera object is moving in
		float angle = 0;
		viewMatrix = viewMatrix * Matrix4::Translation((camMove).Inverse());
		if (camMove.Length() > 1)
		{
			camMove.Normalise();
			Vector3 xProjection = Vector3::PlaneProjection(camMove, Vector3(0, 1, 0));
			angle = Vector3::AngleBetween(xProjection, viewMatrix.GetForwardVector());
		}
		/*viewMatrix = viewMatrix * Matrix4::Translation(camOrigin);
		viewMatrix = viewMatrix * Matrix4::Rotation(angle, Vector3(0, 1, 0));
		viewMatrix = viewMatrix * Matrix4::Translation(camOrigin.Inverse());*/
	}
	//Move depending on the camera actions otherwise
	else
	{
		if (cameraActions.find(MOVE_CAM_UP) != cameraActions.end())
		{
			viewMatrix = Matrix4::Translation(Vector3(0, INV_CAM_MOVEMENT_RATE, 0)) * viewMatrix;
		}
		if (cameraActions.find(MOVE_CAM_DOWN) != cameraActions.end())
		{
			viewMatrix = Matrix4::Translation(Vector3(0, CAM_MOVEMENT_RATE, 0)) * viewMatrix;
		}
		if (cameraActions.find(MOVE_CAM_LEFT) != cameraActions.end())
		{

			viewMatrix = Matrix4::Translation(Vector3(CAM_MOVEMENT_RATE, 0, 0)) * viewMatrix;
		}
		if (cameraActions.find(MOVE_CAM_RIGHT) != cameraActions.end())
		{
			viewMatrix = Matrix4::Translation(Vector3(INV_CAM_MOVEMENT_RATE, 0, 0)) * viewMatrix;
		}
		if (cameraActions.find(MOVE_CAM_BACKWARD) != cameraActions.end())
		{
			viewMatrix = Matrix4::Translation(Vector3(0, 0, INV_CAM_MOVEMENT_RATE)) * viewMatrix;
		}
		if (cameraActions.find(MOVE_CAM_FORWARD) != cameraActions.end())
		{
			viewMatrix = Matrix4::Translation(Vector3(0, 0, CAM_MOVEMENT_RATE)) * viewMatrix;
		}
	}
	Vector3 test = camOrigin;
	viewMatrix = viewMatrix * Matrix4::Translation(camOrigin);

	if (cameraActions.find(TURN_CAM_RIGHT) != cameraActions.end())
	{
		viewMatrix = viewMatrix * Matrix4::Rotation(ROTATION_RATE, Vector3(0, 1, 0));
	}
	if (cameraActions.find(TURN_CAM_LEFT) != cameraActions.end())
	{
		viewMatrix = viewMatrix * Matrix4::Rotation(INV_ROTATION_RATE, Vector3(0, 1, 0));
	}
	if (camRotation != 0)
	{
		viewMatrix = viewMatrix * Matrix4::Rotation(camRotation, Vector3(0, 1, 0));
	}
	viewMatrix = viewMatrix * Matrix4::Translation(camOrigin.Inverse());
	/*for(vector<RenderObject*>::iterator i = renderOb0jects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}*/
}

Vector2	Renderer::GetRenderArea()
{
	return Vector2(width, height);
}