#include "../include/AnimationPlayer.h"


void AnimationPlayer::Render(Renderer* renderer)
{
	// Play active animations
	for(auto it = _activeAnimations.begin(); it != _activeAnimations.end(); it++)	
	{
		it->animation.Render(renderer, it->pos);
	}


	// Remove animations that have already finished playing
	std::vector<PlayingAnimation>::iterator it = _activeAnimations.begin();

	while(it != _activeAnimations.end())
	{
		if(it->animation.IsDone())
		{
			it = _activeAnimations.erase(it);
		}
		else it++;
	}
}


void AnimationPlayer::PlayAnimation(Animation* animation, const Vector2D& pos)
{
	_activeAnimations.push_back({Animation(*animation), pos});
}
