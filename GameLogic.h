#pragma once
#include "Singleton.h"
#include "PhysicsContainer.h"
#include "IGameLogic.h"
#include "StateMachine.h"

class GameLogic : public IGameLogic, public Singleton<GameLogic>
{
	friend class Singleton<GameLogic>;
public:
	bool GameOnToGamePaused();
	bool GameOnToGameOver();
	bool GameOnToGameWon();
	bool GameOnToGameSetup();
	void Update();

private:
	GameLogic();
	~GameLogic();
	void Init();
	PhysicsContainer* physics;
	StateMachine* stateMachine;

};

