#pragma once
#include <string>
#include "RenderObject.h"
#include "LFont.h"

class IProfilerOutputHandler
{
public:
	virtual void BeginOutput() = 0;
	virtual void Sample(float fMin, float fAvg, float fMax,
		int callCount, std::string name, int parentCount) = 0;
	virtual void EndOutput() = 0;
	virtual vector<RenderObject*> PrintOnScreen(LFont* f, Vector3 pos, string text, Shader* shader,...) = 0;
};

