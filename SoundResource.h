#pragma once
#include "Resource.h"
#include "SDL_mixer.h"
#include <string>

using namespace std;

class SoundResource : public Resource
{
public:

	virtual ~SoundResource() {};
	virtual bool Play() = 0;

protected:
};

