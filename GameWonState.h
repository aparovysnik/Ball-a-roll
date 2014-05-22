#pragma once
#include "State.h"
#include "LFont.h"

class GameWonState : public State
{
public:
	GameWonState(string mName, string mDescription);
	~GameWonState();

	void SetRunning(bool reinit);

	void Init();
	void Run();

	void Stop();

private:
	MusicResource* mus;
	LFont *gameWonFont;
	Shader* textShader;
};

