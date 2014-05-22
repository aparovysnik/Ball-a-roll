#pragma once
#include "State.h"
#include "LFont.h"

class GameOverState : public State
{
public:
	GameOverState(string mName, string mDescription);
	~GameOverState();

	void SetRunning(bool reinit);

	void Init();
	void Run();

	void Stop();

private:
	MusicResource* mus;
	LFont *gameOverFont;
	Shader* textShader;
};

