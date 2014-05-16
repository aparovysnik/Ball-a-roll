#pragma once
#ifndef CONTROLS_H
#define CONTROLS_H
#include <set>

using namespace std;
enum Action{ OPTION_SELECT, MOVE_CAM_RIGHT, MOVE_CAM_LEFT, MOVE_CAM_UP, MOVE_CAM_DOWN, MOVE_CAM_FORWARD, MOVE_CAM_BACKWARD,
	TURN_CAM_RIGHT, TURN_CAM_LEFT, JUMP, PAUSE, RESTART };
struct KeyUpdates
{
	set<Action> global;
	set<Action> local;
};

enum ActionScope {GLOBAL, LOCAL};
#endif

