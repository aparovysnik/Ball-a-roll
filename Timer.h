#pragma once
#include "Windows.h"
class Timer
{
public:
	Timer();
	float GetLifetime();
	float GetFrameTime(); //Get the time since the last frametime call
	void StartTimer();

private:
	LARGE_INTEGER frametimeStart;
	LARGE_INTEGER frequency;
	LARGE_INTEGER lifetimeStart;
	float previousFrameTime;

	bool started;
};

