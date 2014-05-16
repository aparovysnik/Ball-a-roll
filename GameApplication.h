#pragma once
#include "Application.h"
#include "GameLogic.h"

class GameApplication : public Application
{
public:

	static void Create(Window *w, Environment *env, string envName) { if (instance == NULL) { instance = new GameApplication(w, env, envName); } };

	void Start();
	void Run();
	void Init();

	LFont* GetApplicationFont() { return appFont; }

private:
	GameApplication(Window *w, Environment *env, string envName);
	~GameApplication();

	LFont *appFont;
	GameLogic* logic;

};

