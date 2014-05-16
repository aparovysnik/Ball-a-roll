#include "Controller.h"


Controller::Controller()
{
	userControls.insert(pair<Action, KeyboardKeys>(PAUSE, KEYBOARD_ESCAPE));
	userControls.insert(pair<Action, KeyboardKeys>(MOVE_CAM_DOWN, KEYBOARD_DOWN));
	userControls.insert(pair<Action, KeyboardKeys>(MOVE_CAM_UP, KEYBOARD_UP));
	userControls.insert(pair<Action, KeyboardKeys>(MOVE_CAM_LEFT, KEYBOARD_LEFT));
	userControls.insert(pair<Action, KeyboardKeys>(MOVE_CAM_RIGHT, KEYBOARD_RIGHT));
	userControls.insert(pair<Action, KeyboardKeys>(MOVE_CAM_BACKWARD, KEYBOARD_S));
	userControls.insert(pair<Action, KeyboardKeys>(MOVE_CAM_FORWARD, KEYBOARD_W));
	userControls.insert(pair<Action, KeyboardKeys>(JUMP, KEYBOARD_SPACE));
	userControls.insert(pair<Action, KeyboardKeys>(TURN_CAM_LEFT, KEYBOARD_A));
	userControls.insert(pair<Action, KeyboardKeys>(TURN_CAM_RIGHT, KEYBOARD_D));
	userControls.insert(pair<Action, KeyboardKeys>(RESTART, KEYBOARD_R));

}


Controller::~Controller()
{
}

void Controller::ActivateAction(Action act, ActionScope scope)
{
	if (!IsActiveAction(act, scope) && (userControls.find(act) != userControls.end()))
	{
		if (scope == LOCAL)
		{
			activeLocalActions.insert(act);
		}
		else
		{
			activeGlobalActions.insert(act);
		}
	}
}

void Controller::DeactivateAction(Action act, ActionScope scope)
{
	if (IsActiveAction(act, scope))
	{
		if (scope == LOCAL)
		{
			activeLocalActions.erase(act);
		}
		else
		{
			activeGlobalActions.erase(act);
		}
	}
}

bool Controller::IsActiveAction(Action act, ActionScope scope)
{
	bool result;

	if (scope == LOCAL)
	{
		result = (activeLocalActions.find(act) != activeLocalActions.end());
	}
	else
	{
		result = (activeGlobalActions.find(act) != activeGlobalActions.end());
	}
	return result;
}

bool Controller::IsContinuous(Action act)
{
	return Window::GetKeyboard()->KeyHeld(userControls.at(act));
}
KeyUpdates Controller::GetActions()
{
	KeyUpdates currentlyPressed;
	for each(Action act in activeGlobalActions)
	{
		if (Window::GetKeyboard()->KeyDown(userControls.at(act)))
		{
			currentlyPressed.global.insert(act);
		}
	}
	for each(Action act in activeLocalActions)
	{
		if (Window::GetKeyboard()->KeyDown(userControls.at(act)))
		{
			currentlyPressed.local.insert(act);
		}
	}
	return currentlyPressed;
}

set<Action> Controller::GetGlobalActions()
{
	set<Action> currentlyPressed;
	for each(Action act in activeGlobalActions)
	{
		if (Window::GetKeyboard()->KeyDown(userControls.at(act)))
		{
			currentlyPressed.insert(act);
		}
	}
	return currentlyPressed;
}

set<Action> Controller::GetLocalActions()
{
	set<Action> currentlyPressed;
	for each(Action act in activeLocalActions)
	{
		if (Window::GetKeyboard()->KeyDown(userControls.at(act)))
		{
			currentlyPressed.insert(act);
		}
	}
	return currentlyPressed;
}
