#include "GameLogic.h"
#include "Controller.h"
#include "Application.h"



GameLogic::GameLogic()
{
	Init();
}


GameLogic::~GameLogic()
{
}

bool GameLogic::GameOnToGamePaused()
{
	set<Action> actions = Controller::Instance().GetLocalActions();
	set<Action>::iterator act;
	act = actions.find(PAUSE);
	return((act != actions.end()) && !(Controller::Instance().IsContinuous(*act)));
}

bool GameLogic::GameOnToGameSetup()
{
	set<Action> actions = Controller::Instance().GetLocalActions();
	set<Action>::iterator act;
	act = actions.find(RESTART);
	return((act != actions.end()) && !(Controller::Instance().IsContinuous(*act)));
}

bool GameLogic::GameOnToGameOver()
{
	Vector3 origin = physics->GetCamObjectOrigin();
	return (origin.y < -200);
}

bool GameLogic::GameOnToGameWon()
{
	vector<GameEntity*> finishPoints = Application::Instance().GetEnvironment()->GetFinishingPoints();

	Vector3 origin = physics->GetCamObjectOrigin();
	for each(GameEntity* finish in finishPoints)
	{
		if((finish->GetPhysics()->GetPosition() - origin).Length() < finish->GetPhysics()->GetScale().x * 0.5)
		{
			return true;
		}
	}
	return false;
}

void GameLogic::Init()
{
	stateMachine = &StateMachine::Instance();

	//Starting state
	State* test0 = new GameSetupState("GAME_SETUP", "Game is being set up");
	State* test1 = new GameOnState("GAME_ON", "Game is running");
	State* test2 = new GamePausedState("GAME_PAUSED", "Game is paused");
	State* test3 = new GameOverState("GAME_OVER", "Game is over");
	State* test4 = new GameWonState("GAME_WON", "Game has been won");
	stateMachine->AddState(test0);
	stateMachine->AddState(test1);
	stateMachine->AddState(test2);
	stateMachine->AddState(test3);
	stateMachine->AddState(test4);

	std::function<bool()> transition = std::bind(&GameLogic::GameOnToGamePaused, this);
	stateMachine->AddTransition("GAME_ON", "GAME_PAUSED", transition);
	stateMachine->AddTransition("GAME_PAUSED", "GAME_ON", transition);
	transition = std::bind(&GameLogic::GameOnToGameOver, this);
	stateMachine->AddTransition("GAME_ON", "GAME_OVER", transition);
	transition = std::bind(&GameLogic::GameOnToGameWon, this);
	stateMachine->AddTransition("GAME_ON", "GAME_WON", transition);
	transition = std::bind(&GameLogic::GameOnToGameSetup, this);
	stateMachine->AddTransition("GAME_ON", "GAME_SETUP", transition);
	stateMachine->Start("GAME_SETUP");
}

void GameLogic::Update()
{
	physics = &PhysicsContainer::Instance();
	string tr = stateMachine->CurrentState()->TransitionTriggered();
	if (!tr.empty())
	{
		stateMachine->Advance(tr);
	}
}
