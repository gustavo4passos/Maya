#ifndef EVILSONIC_H
#define EVILSONIC_H

#include "GameEnemy.h"

class Renderer;

class EvilSonic : public GameEnemy {
public:
	EvilSonic(float x, float y, float w, float h) : 
		GameEnemy(x, y, w, h) {

	}

	void Update() override;
	void Draw(Renderer* renderer, float positionInterpolation) override;
};

#endif
