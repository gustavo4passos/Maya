#include "SoundEffect.h"

#include "ErrorHandler.h"

SoundEffect::SoundEffect() {}
SoundEffect::~SoundEffect() {}

bool LoadSoundEffect(std::string name, std::string path) {
	Mix_Chunk* sample;
	sample = LoadWAV(path.c_str());
	if (!sample) {
		LOG_ERROR("Unable to load the sample: " + Mix_GetError());
		return false;
	}

	SoundEffectsMap.insert(std::pair<std::string, Sound*>(name, sample));
	return true;
}