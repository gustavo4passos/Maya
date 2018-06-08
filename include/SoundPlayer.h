#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef Sound Mix_Chunk;
typedef Music Mix_Music;

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	static bool Init();
	static void Clean();

	// SFX
	static bool PlaySFX(Sound* soundfx, bool loop = false);

	// BGM
	static bool PlayBGM(Music* music, bool loop);
};

#endif