#pragma once
#include "Environment.h"
#include "Renderer.h"

#include "ResourceManager.h"
#include "Controller.h"
#include "PhysicsContainer.h"
#include "StateMachine.h"

#include "Singleton.h"
#include <map>

#include "ft2build.h"
#include FT_FREETYPE_H
#include "Lfont.h"

#include "GameEntity.h"


#include "Logger.h"

#include "EmbeddedResources.h"

class Application : public Singleton<Application>
{
	friend Singleton<Application>;
public:

	Environment* GetEnvironment();

	//Create environment with default parameters
	Environment* CreateEnvironment(string name);

	Environment* CreateEnvironment(const string &name, unsigned int maxCapacity, Vector3 origin, Vector3 halfDim);
	
	Environment* DuplicateEnvironment(string newName);

	Vector2 GetScreenSize() { return window->GetScreenSize(); }

	void SetEnvironment(const string &name);

	LFont* LoadTrueTypeFont(std::string path, GLuint pixelSize, int shrinkRate, Vector4 col[4]);

	virtual void Start();
	virtual void Run() = 0;
	virtual void Stop();

protected:
	bool running;
	Environment* env;
	map<string, Environment*> envs;

	Window* window;
	Renderer *renderer;
	Controller *controller;
	PhysicsContainer *physics;
	StateMachine *stateMachine;
	Logger *logger;

	Timer timer;

	float max_fps;

	FT_Library ft; //FreeType library
	FT_Face ftFace;

	Application(Window *w, Environment *env, string envNam);
	~Application();
	
};

