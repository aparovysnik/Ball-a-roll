#include "GameWonState.h"
#include "ControllableObject.h"
#include "GameApplication.h"


GameWonState::GameWonState(string mName, string mDescription) : State(mName, mDescription)
{
	Vector4 col[4] = { Vector4(0.0, 1.0, 0.0, 1.0), Vector4(0.0, 1.0, 0.5, 1.0), Vector4(0.0, 1.0, 0.7, 1.0), Vector4(0.0, 1.0, 1.0, 1.0) };
	gameWonFont = Application::Instance().LoadTrueTypeFont("arial.ttf", 700, 32, col);
	physics = &PhysicsContainer::Instance();
	renderer = &Renderer::Instance();
	this->environment = GameApplication::Instance().GetEnvironment();
}


GameWonState::~GameWonState()
{
}

void GameWonState::SetRunning(bool reinit)
{
	State::SetRunning(reinit);
	mus = ResourceManager::Instance().GetResource<MusicResource>("Butterfly Tea - Butterfly Odyssey 2013.mp3");
	mus->Play();

	textShader = ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFragTex.glsl")->GetShader();
	Vector3 camOrigin = physics->GetCamObjectOrigin();
	Vector3 camDirection = renderer->GetCamDirection();
	this->environment = GameApplication::Instance().GetEnvironment();
	this->environment->RemoveBodies();
	Vector2 screenSize = Application::Instance().GetScreenSize();


	//vector<RenderObject*> text = gameOverFont->renderText(screenSize.x * 0.5, screenSize.y * 0.5, 0, "GAME OVER", textShader);
	vector<RenderObject*> text = gameWonFont->renderText(camOrigin.x - 30, camOrigin.y, camOrigin.z - 20, "YOU WIN!", textShader);
	RenderObject* textContainer = new RenderObject();
	//textContainer->ToggleViewTransformation(false);
	for each(RenderObject* obj in text)
	{
		//obj->ToggleViewTransformation(false);
		textContainer->AddChild(obj);
	}
	GameEntity* textEntity = new GameEntity(textContainer);
	textEntity->SetTransparency(true);
	environment->AddEntity(textEntity, false);
	renderer->RotateCamera(1.0);
}

void GameWonState::Run()
{
}

void GameWonState::Stop()
{
	renderer->RotateCamera(0);
	mus->Stop();
}

void GameWonState::Init()
{
	State::Init();
}
