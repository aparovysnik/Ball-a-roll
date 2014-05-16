#pragma once
#include "State.h"
#include "../nclgl/Vector3.h"
#include "../nclgl/Vector4.h"

class GamePausedState : public State
{
public:
	GamePausedState(string mName, string mDescription);
	~GamePausedState();

	void SetRunning();
	void Init();
	void Run();

private:
	Shader* dColor;
	Shader* dTexture;
};

