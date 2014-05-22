#pragma once
#include "State.h"
#include "Menu.h"
#include "nclgl/Vector3.h"
#include "nclgl/Vector4.h"

class GameSetupState : public State
{
public:
	GameSetupState(string mName, string mDescription);
	~GameSetupState();

	void SetRunning(bool reinit);

	void Init();
	void Run();

	void Stop();

private:
	void ButtonPlay_Pressed(int i);
	MusicResource* mus;
	Menu* menu;
	Environment* environment;
	bool isComplete = false;
};


