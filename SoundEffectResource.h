#pragma once
#include "SoundResource.h"

class SoundEffectResource : public SoundResource
{
public:
	SoundEffectResource();
	~SoundEffectResource();

	bool Load(const string& sourcePath);
	void Unload();
	bool Play();

private:
	Mix_Chunk *sound;
};

