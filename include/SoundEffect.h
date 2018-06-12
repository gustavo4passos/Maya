#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H 

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef Sound Mix_Chunk;
typedef Music Mix_Music;

class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();

	static bool LoadSample(std::string name, std::string path);

private:
	std::map<std::string, Sound*> SoundEffectsMap;
};

#endif