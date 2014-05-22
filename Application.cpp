#include "Application.h"

Application::Application(Window *w, Environment *env, string envName)
{
	max_fps = ((float)1000 / 65);
	running = false;
	this->env = env;
	envs.insert(pair<string, Environment*>(envName, env));

	//Start SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	this->window = w;

	//Create components
	Renderer::Create(*window, env);
	PhysicsContainer::Create(env);

	Controller::Create();
	ResourceManager::Create();
	Logger::Create();
	StateMachine::Create();

	renderer = &(Renderer::Instance());
	controller = &(Controller::Instance());
	physics = &PhysicsContainer::Instance();
	logger = &Logger::Instance();
	stateMachine = &StateMachine::Instance();


	logger->Write(LOG_APP, (unsigned long)ID_FREETYPE_INIT);
	if (!LFont::initFreeType()) {
		logger->Write(LOG_APP, (unsigned long)ID_FREETYPE_INIT_FAILED);
		exit(1);
	}
}
Application::~Application()
{
}

Environment* Application::GetEnvironment()
{
	return env;
}

void Application::Start()
{
	if (running == false)
	{
		running = true;
		while (window->UpdateWindow() && running)
		{
			Run();
		}
		Stop();
	}
}

void Application::Stop()
{
	running = false;
	ResourceManager::Instance().Destroy();
	Logger::Instance().Destroy();
	Renderer::Instance().Destroy();
	PhysicsContainer::Instance().Destroy();
	Controller::Instance().Destroy();
	StateMachine::Instance().Destroy();

	//Quit SDL Mixer
	Mix_CloseAudio();
	//Quit SDL
	SDL_Quit();
}

Environment* Application::CreateEnvironment(string name)
{
	env = new Environment(env->GetCapacity(), env->GetOrigin(), env->GetHalfDimension());
	envs.erase(name);
	envs.insert(pair<string, Environment*>(name, env));
	return env;
}
Environment* Application::DuplicateEnvironment(string newName)
{
	env = new Environment(*env);
	envs.erase(newName);
	envs.insert(pair<string, Environment*>(newName, env));
	return env;
}
Environment* Application::CreateEnvironment(const string &name, unsigned int maxCapacity, Vector3 origin, Vector3 halfDim)
{
	env = new Environment(maxCapacity, origin, halfDim);
	envs.erase(name);
	envs.insert(pair<string, Environment*>(name, env));
	return env;
}

void Application::SetEnvironment(const string &name)
{
	if (envs[name] != NULL)
	{
		env = envs[name];
		renderer->SetEnvironment(env);
		renderer->Reset();
		physics->SetEnvironment(env);
	}
	cout << "Environment setup complete." << endl;
}


LFont* Application::LoadTrueTypeFont(std::string path, GLuint pixelSize, int shrinkRate, Vector4 col[4])
{
	LFont* font = new LFont();
	if (!font->loadFreeType(path, pixelSize, shrinkRate, col))
	{
		printf("Unable to load ttf font!\n");
		exit(1);
	}
	return font;
}


