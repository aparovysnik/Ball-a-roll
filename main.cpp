#include "Menu.h"
#include "GameApplication.h"
#pragma comment(lib, "nclgl.lib")


int main(int argc, char **argv) {
	GameApplication::Create(new Window(), new Environment(256, Vector3(0,0,0), Vector3(1000,1000,1000)), "MainEnvironment");
	GameApplication::Instance().Start();
	return 1;
}