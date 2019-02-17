#include "../include/AnimationPlayer.h"

void AnimationPlayer::Update() {
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

	// Update active animations
	for(auto it = _activeAnimations.begin(); it != _activeAnimations.end(); it++)	
	{
		it->animation.Update();
	}
}

void AnimationPlayer::Render(Renderer* renderer)
{
	// Play active animations
	for(auto it = _activeAnimations.begin(); it != _activeAnimations.end(); it++)	
	{
		it->animation.Render(renderer, it->pos);
	}
}


void AnimationPlayer::PlayAnimation(Animation* animation, const Vector2D& pos, bool loop)
{
	_activeAnimations.push_back({Animation(*animation), pos});
	_activeAnimations.back().animation.Play(loop);
}
