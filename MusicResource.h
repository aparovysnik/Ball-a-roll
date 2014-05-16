#pragma once
#include "SoundResource.h"

using namespace std;

class MusicResource : public SoundResource
{
public:
	MusicResource();
	~MusicResource();

	bool Play();
	void Pause();
	void Resume();
	bool Stop();
	bool Load(const string& sourcePath);
	void Unload();

	static void PauseMusic();
	static void ResumeMusic();

private:
	Mix_Music *music;
	bool isPlaying;
	static bool globalPause;
};

