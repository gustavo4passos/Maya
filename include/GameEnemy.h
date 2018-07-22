#include "GameObject.h"

class GameEnemy : public GameObject {
public:
	GameEnemy(const CollisionRect& collisionRect, int spriteW, int spriteH) 
	:	GameObject(collisionRect, spriteW, spriteH)
	{ }

	virtual ~GameEnemy() { }

	inline const int damage() { return _damage; }

private:
	int _damage;
};
