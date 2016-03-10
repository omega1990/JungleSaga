#pragma once
#include <sdl\SDL_mixer.h>
#include <iostream>

class MusicMixer
{
public:
	MusicMixer();
	~MusicMixer();

	void PlayMenuMusic() const;
	void PlayGameMusic() const;
	void PlayGemHit() const;
	void StopMusic() const;
	void PlaySwoosh() const;
	void PlayGameOverMusic() const;


private:
	//The gameMusic that will be played
	Mix_Music *menuMusic;
	Mix_Music *gameMusic;
	Mix_Music *gameOver;

	//The sound effects that will be used
	Mix_Chunk *gemHit;
	Mix_Chunk *swoosh;
};

