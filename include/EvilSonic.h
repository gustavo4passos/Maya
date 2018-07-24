#ifndef EVILSONIC_H
#define EVILSONIC_H

#include "GameEnemy.h"

class Renderer;

class EvilSonic : public GameEnemy {
public:
	EvilSonic(const CollisionRect& collisionRect, int spriteW, int spriteH) 
	:	GameEnemy(collisionRect, spriteW, spriteH)
	{ }
	
	virtual ~EvilSonic() { }

	void Update() override;
	void Draw(Renderer* renderer, float deltaTime) override;

	bool OnNotify(Event* event);
};

#endif
