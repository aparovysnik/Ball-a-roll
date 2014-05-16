#pragma once
#include <string>
#include "Singleton.h"

using namespace std;

// Base class for all resources
class Resource
{
public:

	virtual ~Resource() {};
	virtual bool Load(const string & sourcePath) = 0;
	virtual void Unload() = 0;

protected: 
};

