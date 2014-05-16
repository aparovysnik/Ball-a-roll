#include "SoundEffectResource.h"


SoundEffectResource::SoundEffectResource()
{
}


SoundEffectResource::~SoundEffectResource()
{
	Unload();
}

bool SoundEffectResource::Load(const string& sourcePath)
{
	sound = Mix_LoadWAV(sourcePath.c_str());
	return (sound != NULL);
}

bool SoundEffectResource::Play()
{
	return (Mix_PlayChannel(-1, sound, 0) == -1);
}

void SoundEffectResource::Unload()
{
	Mix_FreeChunk(sound);
}