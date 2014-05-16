#pragma once
#include "Resource.h"
#include "Shader.h"
class ShaderResource : public Resource
{
public:
	ShaderResource();
	ShaderResource(const string& addr);

	~ShaderResource();


	bool Load(const string& addr);
	void Unload();

	Shader* GetShader() { return shader; }

private:
	bool GetValue(istringstream &stream, string& s, string addr);

	Shader* shader;
};

