#include "MusicResource.h"

bool MusicResource::globalPause = false;

MusicResource::MusicResource()
{
	isPlaying = false;
}


MusicResource::~MusicResource()
{
	Unload();
}

bool MusicResource::Load(const string& sourcePath)
{
	music = Mix_LoadMUS(sourcePath.c_str());
	return(music != NULL);
}
void MusicResource::Unload()
{
	Mix_FreeMusic(music);
}

bool MusicResource::Play()
{
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (Mix_PlayMusic(music, -1) == -1)
		{
			isPlaying = true;
			return true;
		}
	}
	return false;
}

bool MusicResource::Stop()
{
	if ((Mix_PlayingMusic() != 0) && isPlaying)
	{
		Mix_HaltMusic();
		isPlaying = false;
		return true;
	}
	return false;
}

void MusicResource::Pause()
{
	if (isPlaying)
	{
		Mix_PauseMusic();
		isPlaying = false;
	}
}

void MusicResource::Resume()
{
	if (!isPlaying || globalPause)
	{
		Mix_ResumeMusic();
		isPlaying = true;
	}
}

void MusicResource::PauseMusic()
{
	globalPause = true;
	Mix_PauseMusic();
}

void MusicResource::ResumeMusic()
{
	globalPause = false;
	Mix_ResumeMusic();
}
