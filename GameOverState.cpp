#include "GameOverState.h"
#include "ControllableObject.h"
#include "GameApplication.h"


GameOverState::GameOverState(string mName, string mDescription) : State(mName, mDescription)
{
	gameOverFont = NULL;
	textShader = NULL;
	Vector4 col[4] = { Vector4(1.0, 0, 0.0, 1.0), Vector4(1.0, 0, 0.1, 1.0), Vector4(1.0, 0, 0.2, 1.0), Vector4(1.0, 0.0, 0.3, 1.0) };
	gameOverFont = Application::Instance().LoadTrueTypeFont("arial.ttf", 700, 32, col);
	physics = &PhysicsContainer::Instance();
	renderer = &Renderer::Instance();
	this->environment = GameApplication::Instance().GetEnvironment();
}


GameOverState::~GameOverState()
{
}

void GameOverState::SetRunning(bool reinit)
{
	State::SetRunning(reinit);
	mus = ResourceManager::Instance().GetResource<MusicResource>("Tryad - Lovely.mp3");
	mus->Play();

	textShader = ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFragTex.glsl")->GetShader();
	Vector3 camOrigin = physics->GetCamObjectOrigin();
	Vector3 camDirection = renderer->GetCamDirection();
	this->environment = GameApplication::Instance().GetEnvironment();
	this->environment->RemoveBodies();
	Vector2 screenSize = Application::Instance().GetScreenSize();

	
	//vector<RenderObject*> text = gameOverFont->renderText(screenSize.x * 0.5, screenSize.y * 0.5, 0, "GAME OVER", textShader);
	vector<RenderObject*> text = gameOverFont->renderText(camOrigin.x - 30, camOrigin.y, camOrigin.z - 20,"GAME OVER", textShader);
	RenderObject* textContainer = new RenderObject();
	//textContainer->ToggleViewTransformation(false);
	for each(RenderObject* obj in text)
	{
		//obj->ToggleViewTransformation(false);
		textContainer->AddChild(obj);
	}
	GameEntity* textEntity = new GameEntity(textContainer);
	environment->AddEntity(textEntity, false);
	renderer->RotateCamera(1.0);
}

void GameOverState::Run()
{
}

void GameOverState::Stop()
{
	renderer->RotateCamera(0);
	mus->Stop();
}

void GameOverState::Init()
{
	State::Init();
}
