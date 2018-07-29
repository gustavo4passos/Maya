#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class Enemy : public GameObject {

public:
	Enemy(const CollisionRect& collisionRect, int spriteW, int spriteH);
	Enemy(float x, float y, int w, int h);
	virtual ~Enemy();

	virtual void Draw(Renderer*, float deltaTime);
	virtual void Update();
    virtual bool OnNotify(Event* event);

	inline const int damage() { return _damage; }

protected:
	int _damage;
};

#endif