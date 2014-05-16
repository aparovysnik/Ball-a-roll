#pragma once
#include "State.h"
#include "LFont.h"

class GameOverState : public State
{
public:
	GameOverState(string mName, string mDescription);
	~GameOverState();

	void SetRunning();

	void Init();
	void Run();

private:
	MusicResource* mus;
	LFont *gameOverFont;
	Shader* textShader;
};

