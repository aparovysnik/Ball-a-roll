#include "GameSetupState.h"
#include "ResourceManager.h"
#include "LFont.h"
#include "Application.h"


GameSetupState::GameSetupState(string mName, string mDescription) : State(mName, mDescription)
{
	menu = NULL;
	this->environment = Application::Instance().CreateEnvironment(mName);
	Init();
}


GameSetupState::~GameSetupState()
{
}

void GameSetupState::SetRunning()
{
	cout << "Setup starts!" << endl;
	Application::Instance().SetEnvironment(mName);
}

void GameSetupState::Init()
{
	Shader* bgShader = ResourceManager::Instance().GetResource<ShaderResource>("testVert.glsl,testFragTexCol.glsl")->GetShader();
	Shader* txtShader = ResourceManager::Instance().GetResource<ShaderResource>("testVert.glsl,testFragTex.glsl")->GetShader();
	TextureResource* btnBg = ResourceManager::Instance().GetResource<TextureResource>("ohleonardobybeckas.jpg");
	TextureResource* menuBg = ResourceManager::Instance().GetResource<TextureResource>("perlinnoise.png");
	Vector4 col[4] = { Vector4(0.0, 1.0, 0.0, 1.0), Vector4(0.0, 1.0, 0.5, 1.0), Vector4(0.0, 1.0, 0.7, 1.0), Vector4(0.0, 1.0, 1.0, 1.0) };
	LFont* bgFont = Application::Instance().LoadTrueTypeFont("arial.ttf", 700, 44, col);
	Mesh* m = Mesh::GenerateQuad();
	Vector2 screenSize = Application::Instance().GetScreenSize();
	menu = new Menu(bgShader, bgFont, screenSize.x, screenSize.y, menuBg->GetTextureID());
	Vector3 buttonPosition(screenSize.x * 0.5, screenSize.y * 0.1, 0);
	Vector3 buttonScale(70, 30, 0);
	std::function<void(int)> btnPress = std::bind(&GameSetupState::ButtonPlay_Pressed,this,placeholders::_1);
	string text = "PLAY";
	menu->AddButton(m, buttonPosition, buttonScale, bgShader, txtShader, btnPress, bgFont, text, btnBg->GetTextureID());
	GameEntity* ent = new GameEntity(menu);
	environment->AddEntity(ent, false);
}

void GameSetupState::Run()
{
}

void GameSetupState::ButtonPlay_Pressed(int i)
{
	cout << "Button index is: " << i << endl;
	StateMachine::Instance().Advance("GAME_ON");
}