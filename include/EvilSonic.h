#ifndef EVILSONIC_H
#define EVILSONIC_H

#include <iostream>
#include "GameEnemy.h"

class Renderer;

class EvilSonic : public GameEnemy {
public:
	EvilSonic(float x, float y, float w, float h, float collisionOffsetX = 12, float collisionOffsetY = 5, float collisionRectW = 10, float collisionRectH = 30) 
	: GameEnemy(x, y, w, h, collisionOffsetX, collisionOffsetY, collisionRectW, collisionRectH) { }

	void Update() override;
	void Draw(Renderer* renderer, float positionInterpolation) override;
};

#endif
