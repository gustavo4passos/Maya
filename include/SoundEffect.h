#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H 

class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();
private:
	Mix_Chunk* sample;
};

#endif