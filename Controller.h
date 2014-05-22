#pragma once
#include "nclgl/Window.h"
#include "nclgl/Keyboard.h"
#include "Controls.h"
#include "Singleton.h"
#include <map>
#include <set>

using namespace std;
class Controller : public Singleton<Controller>
{
	friend Singleton<Controller>;

public:

	KeyUpdates GetActions();
	set<Action> GetGlobalActions();
	set<Action> GetLocalActions();
	bool IsContinuous(Action act);
	void ActivateAction(Action act, ActionScope scope);
	void DeactivateAction(Action act, ActionScope scope);
	bool IsActiveAction(Action act, ActionScope scope);

private:
	map<Action, KeyboardKeys> userControls;
	set<Action> activeLocalActions;
	set<Action> activeGlobalActions;

	Controller();
	~Controller();
};

