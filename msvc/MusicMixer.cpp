#include "MusicMixer.h"

MusicMixer::MusicMixer()
	: menuMusic(nullptr)
	, gameMusic(nullptr)
	, gameOver(nullptr)
	, gemHit(nullptr)
	, swoosh(nullptr)
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	menuMusic = Mix_LoadMUS("./assets/music.mp3");
	if (!menuMusic)
		std::cout << "Error while loading music " << Mix_GetError() << std::endl;

	gameMusic = Mix_LoadMUS("./assets/music.mp3");
	if (!gameMusic)
		std::cout << "Error while loading music " << Mix_GetError() << std::endl;

	gemHit = Mix_LoadWAV("./assets/gemPing.wav");
	if (!gemHit)
		std::cout << "Error while loading effect " << Mix_GetError() << std::endl;

	swoosh = Mix_LoadWAV("./assets/swoosh.wav");
	if (!swoosh)
		std::cout << "Error while loading effect " << Mix_GetError() << std::endl;

	gameOver = Mix_LoadMUS("./assets/gameOver.wav");
	if (!gameOver)
		std::cout << "Error while loading music " << Mix_GetError() << std::endl;
}


MusicMixer::~MusicMixer()
{
	Mix_FreeMusic(gameOver);
	Mix_FreeChunk(swoosh);
	Mix_FreeChunk(gemHit);
	Mix_FreeMusic(gameMusic);
	Mix_FreeMusic(menuMusic);
}

void MusicMixer::PlayMenuMusic() const
{
	if (!Mix_PlayingMusic())
	{
		if (Mix_PlayMusic(menuMusic, -1) == -1)
			std::cout << Mix_GetError() << std::endl;
	}
}


void MusicMixer::PlayGameMusic() const
{
	if (!Mix_PlayingMusic())
	{
		if(Mix_PlayMusic(gameMusic, -1) == -1)
			std::cout << Mix_GetError() << std::endl;
	}
}

void MusicMixer::PlayGameOverMusic() const
{
	if (!Mix_PlayingMusic())
	{
		if (Mix_PlayMusic(gameOver, 1) == -1)
			std::cout << Mix_GetError() << std::endl;
	}
}

void MusicMixer::PlayGemHit() const
{
	Mix_PlayChannel(1, gemHit, 0);
}

void MusicMixer::PlaySwoosh() const
{
	StopMusic();
	Mix_PlayChannel(2, swoosh, 0);
}

void MusicMixer::StopMusic() const
{
	Mix_HaltMusic();
}