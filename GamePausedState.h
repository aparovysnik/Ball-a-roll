#pragma once
#include "State.h"
#include "nclgl/Vector3.h"
#include "nclgl/Vector4.h"

class GamePausedState : public State
{
public:
	GamePausedState(string mName, string mDescription);
	~GamePausedState();

	void SetRunning(bool reinit);
	void Init();
	void Run();

	void Stop();

private:
	Shader* dColor;
	Shader* dTexture;
};

