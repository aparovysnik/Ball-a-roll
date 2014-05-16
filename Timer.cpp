#include "Timer.h"
Timer::Timer()
{
	started = false;
	previousFrameTime = 0;
}
void Timer::StartTimer()
{

	QueryPerformanceCounter(&lifetimeStart);
	QueryPerformanceCounter(&frametimeStart);
	QueryPerformanceFrequency(&frequency);

	started = true;
}

float Timer::GetLifetime()
{
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return ((t.QuadPart - lifetimeStart.QuadPart) * 1000.0f / frequency.QuadPart);
}

float Timer::GetFrameTime()
{
	if (!started)
	{
		StartTimer();
	}
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	long test = t.QuadPart - frametimeStart.QuadPart;
	float res = ((float)(t.QuadPart - lifetimeStart.QuadPart) * 10) / (frequency.QuadPart);

	float test2 = res - previousFrameTime;
	previousFrameTime = res;
	return test2;
}
