#pragma once
#include "IProfilerOutputHandler.h"
#include "Logger.h"

#include "EmbeddedResources.h"

class ProfileLogHandler : public IProfilerOutputHandler
{
public:
	ProfileLogHandler()	{};
	~ProfileLogHandler() {};

	void BeginOutput();
	void EndOutput();
	void Sample(float fMin, float fAvg, float fMax,
		int callCount, std::string name, int parentCount);
	vector<RenderObject*> PrintOnScreen(LFont* f, Vector3 pos, string text, Shader* shader...);
};

void ProfileLogHandler::BeginOutput()
{
	Logger::Instance().Write(LOG_APP, ID_PROFILE_HEADER);
	Logger::Instance().Write(LOG_APP, ID_PROFILE_DELIMITER);
}

vector<RenderObject*> ProfileLogHandler::PrintOnScreen(LFont* f, Vector3 pos, string text,Shader* shader...)
{
	vector<RenderObject*> result;
	
	va_list args;
	va_start(args, text);
	char szBuf[1024];
	vsprintf(szBuf, text.c_str(), args);
	result = f->renderText(pos.x, pos.y, pos.z, (string)szBuf, shader);

	return result;
}

void ProfileLogHandler::EndOutput()
{
	Logger::Instance().Write(LOG_APP, "\n");
}

void ProfileLogHandler::Sample(float fMin, float fAvg, float fMax,
	int callCount, std::string name, int parentCount)
{
	char namebuf[256], indentedName[256];
	char avg[16], min[16], max[16], num[16];

	sprintf(avg, "%3.1f", fAvg);
	sprintf(min, "%3.1f", fMin);
	sprintf(max, "%3.1f", fMax);
	sprintf(num, "%3d", callCount);

	strcpy(indentedName, name.c_str());
	for (int indent = 0; indent<parentCount; ++indent)
	{
		sprintf(namebuf, " %s", indentedName);
		strcpy(indentedName, namebuf);
	}

	Logger::Instance().Write(LOG_APP, ID_PROFILE_SAMPLE, min, avg, max, num, indentedName);
}