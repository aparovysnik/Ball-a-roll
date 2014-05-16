#pragma once
#include "State.h"
#include "LFont.h"

class GameWonState : public State
{
public:
	GameWonState(string mName, string mDescription);
	~GameWonState();

	void SetRunning();

	void Init();
	void Run();

private:
	MusicResource* mus;
	LFont *gameWonFont;
	Shader* textShader;
};

