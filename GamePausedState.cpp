#include "GamePausedState.h"
#include "GameApplication.h"

GamePausedState::GamePausedState(string mName, string mDescription) : State(mName, mDescription)
{
	physics = &PhysicsContainer::Instance();
	renderer = &Renderer::Instance();
	dColor = ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFragDarker.glsl")->GetShader();
	dTexture = ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFragTexColDarker.glsl")->GetShader();
	Init();
}


GamePausedState::~GamePausedState()
{
}

void GamePausedState::SetRunning(bool reinit)
{
	State::SetRunning(reinit);

	this->environment = GameApplication::Instance().DuplicateEnvironment(mName);
	this->environment->RemoveBodies();

	vector<GameEntity*> ents = this->environment->GetEntities();

	for each(GameEntity* ent in ents)
	{
		if (ent->HasVisuals())
		{
			RenderObject* rObj = ent->GetVisuals();
			if (rObj->HasTexture())
			{
				rObj->SetShader(dTexture);
			}
			else
			{
				rObj->SetShader(dColor);
			}
		}
	}

	MusicResource::PauseMusic();
	Controller* c = &Controller::Instance();
	GameApplication::Instance().SetEnvironment(GetName());
	c->DeactivateAction(MOVE_CAM_BACKWARD, GLOBAL);
	c->DeactivateAction(MOVE_CAM_BACKWARD, LOCAL);
	c->DeactivateAction(MOVE_CAM_FORWARD, GLOBAL);
	c->DeactivateAction(MOVE_CAM_FORWARD, LOCAL);
	c->DeactivateAction(MOVE_CAM_LEFT, GLOBAL);
	c->DeactivateAction(MOVE_CAM_LEFT, LOCAL);
	c->DeactivateAction(MOVE_CAM_RIGHT, GLOBAL);
	c->DeactivateAction(MOVE_CAM_RIGHT, LOCAL);
	c->DeactivateAction(MOVE_CAM_UP, GLOBAL);
	c->DeactivateAction(MOVE_CAM_UP, LOCAL);
	c->DeactivateAction(MOVE_CAM_DOWN, GLOBAL);
	c->DeactivateAction(MOVE_CAM_DOWN, LOCAL);
	c->DeactivateAction(TURN_CAM_LEFT, GLOBAL);
	c->DeactivateAction(TURN_CAM_LEFT, LOCAL);
	c->DeactivateAction(TURN_CAM_RIGHT, GLOBAL);
	c->DeactivateAction(TURN_CAM_RIGHT, LOCAL);
}
void GamePausedState::Run()
{

}

void GamePausedState::Stop()
{

}

void GamePausedState::Init()
{
	State::Init();
}