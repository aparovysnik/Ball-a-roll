#pragma once
#include "State.h"
#include "../nclgl/Vector3.h"
#include "../nclgl/Vector4.h"

class GameOnState : public State
{
public:
	GameOnState(string mName, string mDescription);
	~GameOnState();

	void SetRunning();

	void Init();
	void Run();

private:
	MusicResource* mus;
};

