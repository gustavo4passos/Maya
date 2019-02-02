#ifndef ANIMATIONPLAYER_H
#define ANIMATIONPLAYER_H

#include <vector>

#include "Animation.h"
#include "Vector2D.h"

class AnimationPlayer {
public:
	AnimationPlayer() { }

	void Update();
	void Render(Renderer* renderer);

	void PlayAnimation(Animation* animation, const Vector2D& pos);
	int NumberOfActiveAnimations() const { return _activeAnimations.size(); }

	struct PlayingAnimation {
		Animation animation;
		Vector2D pos;
	};

private:
	std::vector<PlayingAnimation> _activeAnimations;

};

#endif
