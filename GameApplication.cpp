#pragma once
#include "GameApplication.h"
#include "ProfileSample.h"
#include "Octree.h"
#include "PlainObject.h"
#include "ControllableObject.h"


GameApplication::GameApplication(Window* w, Environment *env, string envName) : Application(w, env, envName)
{
}


GameApplication::~GameApplication()
{
}

void GameApplication::Start()
{
	Init();
	Application::Start();
}

void GameApplication::Run()
{
	int t = timer.GetFrameTime();

	if (t < max_fps)
	{
		Sleep(max_fps - t);
		t = max_fps;
	}
	//ProfileSample* p = new ProfileSample("Main loop");

	//ProfileSample* p0 = new ProfileSample("Logic update");
	//Update game logic
	logic->Update();
	//delete p0;
	// rendering stuff down here

	//Update physics
	//ProfileSample* p1 = new ProfileSample("Physics update");
	physics->UpdateScene(t);
	//delete p1;

	//Update graphics
	//ProfileSample* p2 = new ProfileSample("Graphics update");
	renderer->UpdateScene(t);
	//delete p2;
	/*Vector4 col[4] = { Vector4(0.7, 0, 0.5, 1.0), Vector4(0.7, 0, 0.5, 1.0), Vector4(0.7, 0, 0.5, 1.0), Vector4(0.3, 0.2, 0.5, 1.0) };

	vector<RenderObject*> debug = ProfileSample::PrintOnScreen(appFont, Vector3(-150,0,-400));
	RenderObject* tObj = new RenderObject();
	for each(RenderObject* obj in debug)
	{
		tObj->AddChild(obj);
	}

	GameEntity* text = new GameEntity(tObj);
	text->DeleteAfterRendering(true);
	//env->AddEntity(text, false);
	debug.clear();*/
	//ProfileSample::Output();

	//Render scene
	//ProfileSample* p3 = new ProfileSample("Rendering");
	renderer->ClearBuffers();
	renderer->RenderScene();
	renderer->SwapBuffers();
	//delete p3;

	//DEBUG
	//delete p;
}

void GameApplication::Init()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		exit(1);
	}
	//Create and get reference to game logic container
	GLubyte* pixels = new GLubyte[8];
	GameLogic::Create();

	logic = &GameLogic::Instance();

	renderer->SetProjectionMatrix(Matrix4::Perspective(1.0f, 10000, window->GetScreenSize().x / window->GetScreenSize().y, 45));
	renderer->SetViewMatrix(Matrix4::Translation(Vector3(0, 0, 0)));

	Shader* s2 = ResourceManager::Instance().GetResource<ShaderResource>("testvert.glsl,testFragTex.glsl")->GetShader();
	ProfileSample::shader = s2;
	Vector4 col[4] = { Vector4(0.7, 0, 0.5, 1.0), Vector4(0.7, 0, 0.5, 1.0), Vector4(0.7, 0, 0.5, 1.0), Vector4(0.3, 0.2, 0.5, 1.0) };
	GLubyte* pixels1 = new GLubyte[8];
	//appFont = LoadTrueTypeFont("arial.ttf", 700, 32, col);
}
