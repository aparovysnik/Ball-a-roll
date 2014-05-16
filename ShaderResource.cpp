#include "ShaderResource.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "EmbeddedResources.h"
#include "Logger.h"


ShaderResource::ShaderResource()
{
}
ShaderResource::ShaderResource(const string& addr)
{
	Load(addr);
}

ShaderResource::~ShaderResource()
{
	Unload();
}

bool ShaderResource::Load(const string& addr)
{
	istringstream stream(addr);
	vector<string> values;

	string s;
	while (GetValue(stream, s, addr))
	{
		values.push_back(s);
	}
	if (values.size() < 2)
	{
		return false;
	}

	switch (values.size())
	{
	case 2:
	{
			  shader = new Shader(values[0], values[1]);
			  break;
	}
	case 3:
	{
			  shader = new Shader(values[0], values[1], values[2]);
			  break;
	}
	default:
	{
			  shader = new Shader(values[0], values[1], values[2], values[3]);
			  break;
	}
	}

	return (shader->ShaderLinked());
}

void ShaderResource::Unload()
{
	delete shader;
}

bool ShaderResource::GetValue(istringstream &stream, string& s, string addr)
{
	Logger::Instance().Write(LOG_CLIENT, (unsigned long)ID_INVALID_RESOURCE, addr.c_str());
	return ((bool)getline(stream, s, ','));
}
